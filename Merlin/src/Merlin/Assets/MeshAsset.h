#pragma once

#include "Asset.h"
#include "Merlin/Core/Logger.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace Merlin {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 uv;
	};

	class MeshAsset : public Asset {
	public:
		ASSET_TYPE(MeshAsset)

		MeshAsset(const std::string& name, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
			: Asset(name), m_Vertices(std::move(vertices)), m_Indices(std::move(indices)) {
			upload();
		}

		~MeshAsset() {
			if (m_EBO) glDeleteBuffers(1, &m_EBO);
			if (m_VBO) glDeleteBuffers(1, &m_VBO);
			if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		}

		void Bind() const { glBindVertexArray(m_VAO); }
		void Unbind() const { glBindVertexArray(0); }

		void Draw() const {
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		size_t GetVertexCount() const { return m_Vertices.size(); }
		size_t GetIndexCount() const { return m_Indices.size(); }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indices;

		unsigned int m_VAO = 0;
		unsigned int m_VBO = 0;
		unsigned int m_EBO = 0;

		void upload() {
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);

			glBindVertexArray(m_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

			// position
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

			// normal
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			// tangent
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

			// bitangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

			// uv
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

			glBindVertexArray(0);
		}
	};
}