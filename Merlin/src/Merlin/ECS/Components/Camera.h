#pragma once

#include "Merlin/ECS/Component.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Merlin {
	struct Camera : public Component {
		COMPONENT_TYPE(Camera)

		float fov = 45.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		bool isActive = true;

		glm::mat4 GetViewMatrix(const Transform& transform) const {
			glm::vec3 front = GetFront(transform);
			return glm::lookAt(transform.position, transform.position + front, GetUp(transform));
		}

		glm::mat4 GetProjectionMatrix(float aspectRatio) const {
			return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		}

		glm::vec3 GetFront(const Transform& transform) const {
			glm::vec3 front;
			front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
			front.y = sin(glm::radians(transform.rotation.x));
			front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
			return glm::normalize(front);
		}

		glm::vec3 GetRight(const Transform& transform) const {
			return glm::normalize(glm::cross(GetFront(transform), glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		glm::vec3 GetUp(const Transform& transform) const {
			return glm::normalize(glm::cross(GetRight(transform), GetFront(transform)));
		}
	};
}