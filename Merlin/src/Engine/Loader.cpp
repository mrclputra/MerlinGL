#define STB_IMAGE_IMPLEMENTATION
#include "Engine/Loader.h"
#include "Engine/components/Transform.h"
#include "Engine/components/Mesh.h"
#include "Engine/components/Material.h"

namespace Merlin {

void Loader::load(const std::string &path) {
   SPDLOG_INFO("assimp load: {}", path);
   loadFuture = std::async(std::launch::async, [this, path] { loadWorker(path); });
}

void Loader::poll(entt::registry &registry) {
   if (!loadFuture.valid())
      return;
   if (loadFuture.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
      return;
   loadFuture.get();
   upload(registry);
   SPDLOG_INFO("model loaded successfully");
}

void Loader::loadWorker(const std::string &path) {
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path,
      aiProcess_Triangulate |
      aiProcess_GenSmoothNormals |
      aiProcess_FlipUVs |
      aiProcess_CalcTangentSpace // for textures
   );

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      SPDLOG_ERROR("failed to load model, reason: {}", importer.GetErrorString());
      return;
   }

   SPDLOG_INFO("model info: {} meshes, {} materials, {} textures",
      scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

   // assimp provides relative file path references
   // as such we need to process with the directory in mind as well so we can retrieve said images
   std::string directory;
   size_t lastSlash = path.find_last_of("/\\");
   directory = (lastSlash != std::string::npos) ? path.substr(0, lastSlash + 1) : "";

   // todo: see if we need to implement conditional to process embedded textures
   // todo:    i.e. compiled binaries support, does not require directory though

   processNode(scene->mRootNode, scene, glm::mat4(1.0f), directory); // recursive
}

void Loader::processNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, const std::string& directory) {
   glm::mat4 nodeTransform = convertMatrix(node->mTransformation);
   glm::mat4 globalTransform = parentTransform * nodeTransform; // relative to 0,0,0

   for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(mesh, scene, globalTransform, directory);
   }

   for (unsigned int i = 0; i < node->mNumChildren; i++) {
      // recursively process children
      processNode(node->mChildren[i], scene, globalTransform, directory);
   }
}

void Loader::processMesh(const aiMesh *mesh, const aiScene* scene, const glm::mat4& globalTransform, const std::string& directory) {
   SPDLOG_INFO("mesh '{}': {} verts, {} faces", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);
   // if (!mesh->HasNormals())
   //    SPDLOG_WARN("mesh '{}' has no normals", mesh->mName.C_Str());
   // if (!mesh->HasTextureCoords(0))
   //    SPDLOG_WARN("mesh '{}' has no UVs", mesh->mName.C_Str());

   MeshData meshData;
   meshData.vertices.reserve(mesh->mNumVertices);

   meshData.transform = globalTransform;

   // process each vertex in a mesh
   for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;

      // position
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

      // normals
      if (mesh->HasNormals()) {
         SPDLOG_INFO("found normals for mesh {}", mesh->mName.C_Str());
         vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      }
      if (mesh->HasTangentsAndBitangents()) {
         vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
         vertex.bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
      }
      // uvs
      if (mesh->HasTextureCoords(0)) {
         SPDLOG_INFO("found UVs for mesh {}", mesh->mName.C_Str());
         vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }

      meshData.vertices.push_back(vertex);
   }

   // process indices
   for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace& face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
         meshData.indices.push_back(face.mIndices[j]);
      }
   }


   // load textures
   if (mesh->mMaterialIndex < scene->mNumMaterials) {
      aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

      // base color
      aiColor4D color;
      if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
         SPDLOG_INFO("mesh '{}' AI_MATKEY_COLOR_DIFFUSE found: {}, {}, {}", mesh->mName.C_Str(), color.r, color.g, color.b);
         meshData.material.albedo = glm::vec3(color.r, color.g, color.b);
      }

      // diffuse map
      aiString texPath;
      if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
         SPDLOG_INFO("mesh '{}' aiTextureType_DIFFUSE found: {}", mesh->mName.C_Str(), texPath.C_Str());
         meshData.material.albedoMap = std::make_shared<Texture>(directory + texPath.C_Str());
      }
      // normal map
      if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS) {
         SPDLOG_INFO("mesh '{}' aiTextureType_NORMALS found: {}", mesh->mName.C_Str(), texPath.C_Str());
         meshData.material.normalMap = std::make_shared<Texture>(directory + texPath.C_Str());
      }
   }

   meshLoadQueue.emplace(meshData); // add to upload queue
}
void Loader::upload(entt::registry& registry) {
   // loops through all meshData in the queue and uploads
   while (!meshLoadQueue.empty()) {
      auto meshData = meshLoadQueue.front(); // this creates a copy so we should be able to pop safely
      meshLoadQueue.pop();

      uint32_t vao, vbo, ebo;
      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
      glGenBuffers(1, &ebo);

      glBindVertexArray(vao);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);

      // attributes ---
      // position
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
      glEnableVertexAttribArray(0);
      // normal
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
      glEnableVertexAttribArray(1);
      //tangent
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
      glEnableVertexAttribArray(2);
      // bitangent
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
      glEnableVertexAttribArray(3);
      // uv
      glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
      glEnableVertexAttribArray(4);

      glBindVertexArray(0);

      // upload textures
      if (meshData.material.albedoMap)
         meshData.material.albedoMap->upload();
      if (meshData.material.normalMap)
         meshData.material.normalMap->upload();

      // decompose world transform into position rotation and scale
      // note that this is an experimental feature, we may lose it in the future
      glm::vec3 position, scale, skew;
      glm::vec4 perspective;
      glm::quat orientation;
      glm::decompose(meshData.transform, scale, orientation, position, skew, perspective);

      Transform t;
      t.position = position;
      t.scale = scale;
      t.rotation = glm::degrees(glm::eulerAngles(orientation)); // quat -> euler -> degrees, maybe a different convention would be good?

      auto e = registry.create();
      registry.emplace<Transform>(e, t);
      registry.emplace<Mesh>(e, vao, vbo, ebo, static_cast<uint32_t>(meshData.indices.size()));
      registry.emplace<Material>(e, std::move(meshData.material));
      SPDLOG_INFO("uploaded registry entity {} @ ({}, {}, {})", static_cast<uint32_t>(e), t.position.x, t.position.y, t.position.z);
   }
}

void Loader::wipe(entt::registry &registry) {
   // wipes the rendering buffer
   // wipes the registry
   auto view = registry.view<Mesh>();
   for (auto entity : view) {
      auto& mesh = view.get<Mesh>(entity);
      glDeleteVertexArrays(1, &mesh.vao);
      glDeleteBuffers(1, &mesh.vbo);
      glDeleteBuffers(1, &mesh.ebo);
   }
   registry.destroy(view.begin(), view.end());
}

glm::mat4 Loader::convertMatrix(const aiMatrix4x4& from) {
   glm::mat4 to;
   //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
   to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
   to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
   to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
   to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
   return to;
}

}  // namespace Merlin
