#ifndef MERLINGL_LOADER_H
#define MERLINGL_LOADER_H

#include "components/Material.h"

namespace Merlin {

struct Vertex {
   glm::vec3 position{0.0f};
   glm::vec3 normal{0.0f};
   glm::vec3 tangent{0.0f};
   glm::vec3 bitangent{0.0f};
   glm::vec2 uv{0.0f};

   // point cloud only
   glm::vec3 color{1.0f, 0.0f, 1.0f};
};

// existing load functions needs to write to this struct
// then we reuse it in the done function
struct ModelData {
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
   glm::mat4 transform;
   Material material;
};

class Loader {
public:
   void load(const std::string& path);
   void poll(entt::registry& registry); // call this each frame; uploads when ready
   void wipe(entt::registry& registry);

private:
   std::queue<ModelData> modelLoadQueue;
   std::atomic<bool> loadDone{false};
   std::jthread loadThread;

   void loadWorker(const std::string& path, const std::stop_token &token);
   void processNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, const std::string& directory, const std::stop_token& token);
   void processMesh(const aiMesh *mesh, const aiScene* scene, const glm::mat4& globalTransform, const std::string& directory);
   void processPCD(const aiMesh *pcd, const glm::mat4& globalTransform); // no textures

   // upload loaded data to the gpu
   void upload(entt::registry& registry);

   // assimp matrix to opengl, necessary
   // https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/assimp_glm_helpers.h
   static inline glm::mat4 convertMatrix(const aiMatrix4x4& from);
};

}  // namespace Merlin

#endif  // MERLINGL_LOADER_H
