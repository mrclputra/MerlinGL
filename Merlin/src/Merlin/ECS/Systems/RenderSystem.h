#pragma once

#include "Merlin/ECS/System.h"
#include "Merlin/ECS/Registry.h"
#include "Merlin/ECS/Components/Transform.h"
#include "Merlin/ECS/Components/MeshRenderer.h"
#include "Merlin/Rendering/Framebuffer.h"
#include "Merlin/Core/Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>

namespace Merlin {
	class RenderSystem : public System {
	public:
		RenderSystem() : System("RenderSystem") {}

		void Render(Framebuffer& fb) {
			fb.Bind();
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			float aspectRatio = (float)fb.GetWidth() / (float)fb.GetHeight();
			glm::mat4 view = glm::lookAt(m_CameraPosition, m_CameraTarget, m_CameraUp);
			glm::mat4 projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearPlane, m_FarPlane);

			BuildRenderQueue();
			SortRenderQueue();
			FlushRenderQueue(view, projection);

			fb.Unbind();
		}

		void SetCameraPosition(const glm::vec3& pos) { m_CameraPosition = pos; }
		void SetCameraTarget(const glm::vec3& target) { m_CameraTarget = target; }

	private:
		struct RenderCommand {
			EntityID entity;
			unsigned int shaderID;
			unsigned int textureID;
		};

		std::vector<RenderCommand> m_RenderQueue;

		void BuildRenderQueue() {
			m_RenderQueue.clear();

			auto entities = m_Registry->GetEntitiesWithComponent<MeshRenderer>();
			m_RenderQueue.reserve(entities.size());

			for (EntityID entity : entities) {
				if (!m_Registry->HasComponent<Transform>(entity))
					continue;

				auto& renderer = m_Registry->GetComponent<MeshRenderer>(entity);
				if (!renderer.mesh || !renderer.shader)
					continue;

				RenderCommand cmd;
				cmd.entity = entity;
				cmd.shaderID = renderer.shader->GetID();
				cmd.textureID = renderer.material.hasAlbedoMap() ? renderer.material.albedoMap->GetID() : 0;

				m_RenderQueue.push_back(cmd);
			}
		}

		void SortRenderQueue() {
			std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
				if (a.shaderID != b.shaderID)
					return a.shaderID < b.shaderID;
				return a.textureID < b.textureID;
			});
		}

		void FlushRenderQueue(const glm::mat4& view, const glm::mat4& projection) {
			unsigned int currentShader = 0;
			unsigned int currentTexture = 0;

			for (const auto& cmd : m_RenderQueue) {
				auto& transform = m_Registry->GetComponent<Transform>(cmd.entity);
				auto& renderer = m_Registry->GetComponent<MeshRenderer>(cmd.entity);

				if (cmd.shaderID != currentShader) {
					renderer.shader->Bind();
					renderer.shader->SetMat4("u_View", view);
					renderer.shader->SetMat4("u_Projection", projection);
					renderer.shader->SetInt("u_Albedo", 0);
					currentShader = cmd.shaderID;
					currentTexture = 0;
				}

				if (cmd.textureID != currentTexture && cmd.textureID != 0) {
					renderer.material.albedoMap->Bind(0);
					currentTexture = cmd.textureID;
				}

				renderer.shader->SetMat4("u_Model", transform.getModelMatrix());
				renderer.mesh->Draw();
			}

			glUseProgram(0);
		}

		glm::vec3 m_CameraPosition = glm::vec3(2.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraTarget = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_FOV = 45.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 100.0f;
	};
}
