#pragma once

#include "TextureAsset.h"
#include <glm/glm.hpp>
#include <memory>

namespace Merlin {
	struct Material {
		glm::vec4 albedo = glm::vec4(1.0f);
		float metallic = 0.0f;
		float roughness = 0.5f;

		std::shared_ptr<TextureAsset> albedoMap;
		std::shared_ptr<TextureAsset> normalMap;
		std::shared_ptr<TextureAsset> metallicRoughnessMap;

		bool hasAlbedoMap() const { return albedoMap != nullptr; }
		bool hasNormalMap() const { return normalMap != nullptr; }
		bool hasMetallicRoughnessMap() const { return metallicRoughnessMap != nullptr; }
	};
}
