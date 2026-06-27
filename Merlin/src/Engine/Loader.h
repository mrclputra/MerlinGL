#ifndef MERLINGL_LOADER_H
#define MERLINGL_LOADER_H

namespace Merlin {

struct Vertex {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 uv;
};

// existing load functions needs to write to this struct
// then we reuse it in the done function
struct MeshData {
   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
};

class Loader {
public:
   void load(const std::string& path);
   void poll(entt::registry& registry); // call this each frame; uploads when ready
   void wipe(entt::registry& registry);

private:
   std::future<void> loadFuture;
   std::queue<MeshData> meshLoadQueue;

   void loadWorker(const std::string& path);
   void processNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
   void processMesh(const aiMesh* mesh, const glm::mat4& worldTransform);
   // todo: add texture loader

   // this function will upload the loaded data to the gpu
   // maybe should name it upload() instead?
   void done(entt::registry& registry);

   // assimp matrix to opengl, necessary
   // https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/assimp_glm_helpers.h
   static inline glm::mat4 convertMatrix(const aiMatrix4x4& from);
};

}  // namespace Merlin

#endif  // MERLINGL_LOADER_H
