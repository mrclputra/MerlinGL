#ifndef MERLINGL_LOADER_H
#define MERLINGL_LOADER_H

namespace Merlin {

// todo: move to proper place
struct Vertex {
   glm::vec3 position;
   glm::vec3 normal;
   glm::vec2 uv;
};

// this class should not own anything but rather write to existing datastructures
// todo: implement multithreading; refer to my Calder engine on how to do this with std::thread
//    -> just make sure the OpenGL api upload calls are back in the main thread
class Loader {
public:
   static void load(const std::string& path, entt::registry& registry);
   static void wipe(entt::registry& registry);

private:
   static void processNode(const aiNode* node, const aiScene* scene, const glm::mat4& parentTransform, entt::registry& registry);
   static void processMesh(const aiMesh* mesh, const glm::mat4& worldTransform, entt::registry& registry);
   // todo: add texture loader

   // assimp matrix to opengl, necessary
   // https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/assimp_glm_helpers.h
   static inline glm::mat4 convertMatrix(const aiMatrix4x4& from);
};

}  // namespace Merlin

#endif  // MERLINGL_LOADER_H
