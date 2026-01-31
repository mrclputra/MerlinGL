#pragma once

#include "Asset.h"
#include "Merlin/Logger.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace Merlin {
	class MERLIN_API Shader : public Asset {
	public:
		ASSET_TYPE(Shader)

		Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) 
			: Asset(name), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath) {
			if (!compile()) {
				MERLIN_CORE_ERROR("Shader compilation failed: {0}", name);
			}
		}

		~Shader() {
			if (m_ID != 0 && glIsProgram(m_ID)) {
				glDeleteProgram(m_ID);
			}
		}

		void Bind() const { glUseProgram(m_ID); }
		void Unbind() const { glUseProgram(0); }

		unsigned int GetID() const { return m_ID; }

		// uniform setters
		void SetBool(const std::string& name, bool value) const {
			glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
		}
		void SetInt(const std::string& name, int value) const {
			glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
		}
		void SetFloat(const std::string& name, float value) const {
			glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
		}
		void SetVec2(const std::string& name, const glm::vec2& v) const {
			glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &v[0]);
		}
		void SetVec3(const std::string& name, const glm::vec3& v) const {
			glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &v[0]);
		}
		void SetVec4(const std::string& name, const glm::vec4& v) const {
			glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &v[0]);
		}
		void SetMat3(const std::string& name, const glm::mat3& m) const {
			glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
		}
		void SetMat4(const std::string& name, const glm::mat4& m) const {
			glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &m[0][0]);
		}

	private:
		unsigned int m_ID = 0;
		std::string m_VertexPath;
		std::string m_FragmentPath;

		std::string readFile(const std::string& path) {
			std::ifstream file(path);
			if (!file.is_open()) {
				MERLIN_CORE_ERROR("Shader File not found: {0}", path);
				return "";
			}
			std::stringstream buffer;
			buffer << file.rdbuf();
			return buffer.str();
		}

		unsigned int compileShader(const std::string& source, GLenum type) {
			unsigned int shader = glCreateShader(type);
			const char* src = source.c_str();
			glShaderSource(shader, 1, &src, nullptr);
			glCompileShader(shader);

			GLint success;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				GLchar infoLog[1024];
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				MERLIN_CORE_ERROR("Shader compile error: {0}", infoLog);
				glDeleteShader(shader);
				return 0;
			}
			return shader;
		}

		bool compile() {
			std::string vertexCode = readFile(m_VertexPath);
			std::string fragmentCode = readFile(m_FragmentPath);
		
			unsigned int vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
			if (vertex == 0) return false;

			unsigned int fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
			if (fragment == 0) {
				glDeleteShader(vertex);
				return false;
			}

			unsigned int program = glCreateProgram();
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);

			GLint success;
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (!success) {
				GLchar infoLog[1024];
				glGetProgramInfoLog(program, 1024, nullptr, infoLog);
				MERLIN_CORE_ERROR("Shader link error:\n{0}", infoLog);

				glDeleteProgram(program);
				glDeleteShader(vertex);
				glDeleteShader(fragment);
				return false;
			}

			glDeleteShader(vertex);
			glDeleteShader(fragment);


			if (m_ID != 0) glDeleteProgram(m_ID); // overwrite
			m_ID = program;
			return true;
		}
	};
}