#pragma once

#include "Merlin/ECS/System.h"
#include "Merlin/ECS/Registry.h"
#include "Merlin/ECS/Components/Transform.h"
#include "Merlin/ECS/Components/MeshRenderer.h"
#include "Merlin/Rendering/Framebuffer.h"
#include "Merlin/Core/Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// all this stuff is currently placeholder

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

			auto entities = m_Registry->GetEntitiesWithComponent<MeshRenderer>();
			for (EntityID entity : entities) {
				if (!m_Registry->HasComponent<Transform>(entity))
					continue;

				auto& transform = m_Registry->GetComponent<Transform>(entity);
				auto& renderer = m_Registry->GetComponent<MeshRenderer>(entity);

				if (!renderer.mesh || !renderer.shader)
					continue;

				glm::mat4 model = transform.getModelMatrix();

				renderer.shader->Bind();
				renderer.shader->SetMat4("u_Model", model);
				renderer.shader->SetMat4("u_View", view);
				renderer.shader->SetMat4("u_Projection", projection);

				renderer.mesh->Draw();
				renderer.shader->Unbind();
			}

			fb.Unbind();
		}

		void SetCameraPosition(const glm::vec3& pos) { m_CameraPosition = pos; }
		void SetCameraTarget(const glm::vec3& target) { m_CameraTarget = target; }

	private:
		// these are all placeholders
		//	these values are to be replaced with an actual camera class implementation later on
		glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
		glm::vec3 m_CameraTarget = glm::vec3(0.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_FOV = 45.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 100.0f;
	};
}
