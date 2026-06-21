#include "Shader.h"

namespace Merlin {

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {
   GLuint vert = compile(GL_VERTEX_SHADER, readFile(vertex_path));
   GLuint frag = compile(GL_FRAGMENT_SHADER, readFile(fragment_path));

   m_Program = glCreateProgram();
   glAttachShader(m_Program, vert);
   glAttachShader(m_Program, frag);
   glLinkProgram(m_Program);

   GLint ok = GL_FALSE;
   glGetProgramiv(m_Program, GL_LINK_STATUS, &ok);
   if (!ok) {
      GLchar infoLog[1024];
      glGetProgramInfoLog(m_Program, 1024, nullptr, infoLog);
      SPDLOG_ERROR("PROGRAM_LINKING_ERROR: {}", infoLog);
      glDeleteProgram(m_Program);
      glDeleteShader(vert);
      glDeleteShader(frag);
      throw std::runtime_error("PROGRAM_LINKING_ERROR");  // todo: find a better error handling method
   }

   glDeleteShader(vert);
   glDeleteShader(frag);
}

Shader::~Shader() {
   glDeleteProgram(m_Program);
}

void Shader::bind() {
   glUseProgram(m_Program);
}

void Shader::unbind() {
   glUseProgram(0);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
   glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
   glUniformMatrix3fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
   glUniformMatrix2fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const {
   glUniform4fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
   glUniform3fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec2(const std::string &name, const glm::vec2 &vec) const {
   glUniform2fv(glGetUniformLocation(m_Program, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setInt(const std::string &name, int value) const {
   glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
   glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

GLuint Shader::compile(GLenum type, const std::string &source) const {
   GLuint shader = glCreateShader(type);
   const char *c = source.c_str();
   glShaderSource(shader, 1, &c, nullptr);
   glCompileShader(shader);
   GLint ok = GL_FALSE;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
   if (!ok) {
      GLchar infoLog[1024];
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      SPDLOG_ERROR("SHADER_COMPILE_ERROR: {}", infoLog);
      glDeleteShader(shader);
      return 0;
   }
   return shader;  // success
}

// TODO: move to a global utils file
std::string Shader::readFile(const std::string &path) {
   std::ifstream file(path);
   if (!file) {
      SPDLOG_ERROR("Could not open file: {}", path);
      throw std::runtime_error("Could not open file: " + path);
   }
   std::stringstream ss;
   ss << file.rdbuf();
   return ss.str();
}

}  // namespace Merlin