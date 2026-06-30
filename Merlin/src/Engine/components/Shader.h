#ifndef MERLINGL_SHADER_H
#define MERLINGL_SHADER_H

namespace Merlin {
class Shader {
 public:
   Shader(const std::string &vertex_path, const std::string &fragment_path);
   ~Shader();

   void bind();
   void unbind();

   // uniforms
   void setMat4(const std::string &name, const glm::mat4 &mat) const;
   void setMat3(const std::string &name, const glm::mat3 &mat) const;
   void setMat2(const std::string &name, const glm::mat2 &mat) const;
   void setVec4(const std::string &name, const glm::vec4 &vec) const;
   void setVec3(const std::string &name, const glm::vec3 &vec) const;
   void setVec2(const std::string &name, const glm::vec2 &vec) const;
   void setInt(const std::string &name, int value) const;
   void setFloat(const std::string &name, float value) const;

 private:
   GLuint program = 0;

   GLuint compile(GLenum type, const std::string &source) const;
   static std::string readFile(const std::string &path);
};
}  // namespace Merlin
#endif  // MERLINGL_SHADER_H