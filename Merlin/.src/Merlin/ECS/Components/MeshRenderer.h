#pragma once

#include "Merlin/ECS/Component.h"
#include "Merlin/Assets/MeshAsset.h"
#include "Merlin/Assets/ShaderAsset.h"
#include "Merlin/Assets/Material.h"

#include <memory>

namespace Merlin {
	struct MeshRenderer : public Component {
	public:
		COMPONENT_TYPE(MeshRenderer)

		MeshRenderer() = default;
		MeshRenderer(std::shared_ptr<MeshAsset> mesh, std::shared_ptr<ShaderAsset> shader)
			: mesh(mesh), shader(shader) {}

		std::shared_ptr<MeshAsset> mesh;
		std::shared_ptr<ShaderAsset> shader;
		Material material;
	};
}
