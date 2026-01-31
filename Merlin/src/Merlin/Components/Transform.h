#pragma once

#include "Merlin/ECS/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Merlin {
	struct Transform : public Component {
		COMPONENT_TYPE(Transform)
		Transform() {}
		
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f); // degrees
		glm::vec3 scale = glm::vec3(1.0f);
	
		inline void translate(const glm::vec3& delta) { position += delta; }
		inline void rotate(const glm::vec3& delta) { rotation += delta; }
		inline void rescale(const glm::vec3& factor) { scale *= factor; }

		// object space to world space
		glm::mat4 getModelMatrix() const {
			glm::mat4 model(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, scale);
			return model;
		}
	};
}