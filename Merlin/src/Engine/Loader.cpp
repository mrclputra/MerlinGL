#include "Engine/Loader.h"
#include "Engine/components/Transform.h"
#include "Engine/components/Mesh.h"
#include "Engine/components/Material.h"

namespace Merlin {

void Loader::load(const std::string &path, entt::registry &registry) {
   SPDLOG_INFO("assimp load: {}", path);

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

   SPDLOG_INFO("model info: {} meshes,", scene->mNumMeshes);

   // this might be needed later when we do textures and multi-file references
   // std::string directory;
   // size_t lastSlash = path.find_last_of('/');
   // directory = (lastSlash != std::string::npos) ? path.substr(0, lastSlash + 1) : "";

   processNode(scene->mRootNode, scene, glm::mat4(1.0f), registry); // recursive

   SPDLOG_INFO("model loaded successfully");
}

void Loader::processNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, entt::registry &registry) {
   glm::mat4 nodeTransform = convertMatrix(node->mTransformation);
   glm::mat4 globalTransform = parentTransform * nodeTransform;

   for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      processMesh(mesh, globalTransform, registry);
   }

   for (unsigned int i = 0; i < node->mNumChildren; i++) {
      // recursively process children
      processNode(node->mChildren[i], scene, globalTransform, registry);
   }
}

void Loader::processMesh(const aiMesh *mesh, const glm::mat4 &worldTransform, entt::registry &registry) {
   SPDLOG_INFO("mesh '{}': {} verts, {} faces", mesh->mName.C_Str(), mesh->mNumVertices, mesh->mNumFaces);
   if (!mesh->HasNormals())
      SPDLOG_WARN("mesh '{}' has no normals", mesh->mName.C_Str());
   if (!mesh->HasTextureCoords(0))
      SPDLOG_WARN("mesh '{}' has no UVs", mesh->mName.C_Str());

   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   vertices.reserve(mesh->mNumVertices);

   // process each vertex in a mesh
   for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;

      // position
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
      // normals
      if (mesh->HasNormals()) {
         vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
      }
      // uvs
      if (mesh->HasTextureCoords(0)) {
         vertex.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }

      vertices.push_back(vertex);
   }

   // process indices
   for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
      aiFace& face = mesh->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
         indices.push_back(face.mIndices[j]);
      }
   }

   uint32_t vao, vbo, ebo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

   // attributes
   // position
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
   glEnableVertexAttribArray(0);
   // normal
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
   glEnableVertexAttribArray(1);
   // uv
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
   glEnableVertexAttribArray(2);

   glBindVertexArray(0);

   // decompose world transform into position/rotation/scale for the Transform component
   glm::vec3 position, scale, skew;
   glm::vec4 perspective;
   glm::quat orientation;
   glm::decompose(worldTransform, scale, orientation, position, skew, perspective);

   Transform t;
   t.position = position;
   t.scale    = scale;
   t.rotation = glm::degrees(glm::eulerAngles(orientation)); // quat -> euler -> degrees

   auto e = registry.create();
   registry.emplace<Transform>(e, t);
   registry.emplace<Mesh>(e, vao, vbo, ebo, (uint32_t)indices.size());
   registry.emplace<Material>(e); // todo: load from assimp material when texture support is added
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
   registry.clear();
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
