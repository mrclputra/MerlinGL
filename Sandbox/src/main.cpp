#include <Merlin.h>
#include <Merlin/EntryPoint.h>

#include "Editor.h"

// this is placeholder,
// atleast until I get Assimp up and running
std::shared_ptr<Merlin::MeshAsset> CreateCubeMesh(Merlin::AssetManager& assets) {
	std::vector<Merlin::Vertex> vertices = {
		// +Z
		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1,0,0}, {0,1,0}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1,0,0}, {0,1,0}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1,0,0}, {0,1,0}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1,0,0}, {0,1,0}, {0.0f, 1.0f}},
		// -Z
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {-1,0,0}, {0,1,0}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {-1,0,0}, {0,1,0}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {-1,0,0}, {0,1,0}, {1.0f, 1.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {-1,0,0}, {0,1,0}, {0.0f, 1.0f}},
		// +Y
		{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1,0,0}, {0,0,1}, {0.0f, 0.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1,0,0}, {0,0,1}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1,0,0}, {0,0,1}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1,0,0}, {0,0,1}, {0.0f, 1.0f}},
		// -Y
		{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1,0,0}, {0,0,-1}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1,0,0}, {0,0,-1}, {1.0f, 0.0f}},
		{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1,0,0}, {0,0,-1}, {1.0f, 1.0f}},
		{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1,0,0}, {0,0,-1}, {0.0f, 1.0f}},
		// +X
		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0,0,1}, {0,1,0}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0,0,1}, {0,1,0}, {1.0f, 0.0f}},
		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0,0,1}, {0,1,0}, {1.0f, 1.0f}},
		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0,0,1}, {0,1,0}, {0.0f, 1.0f}},
		// -X
		{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0,0,-1}, {0,1,0}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0,0,-1}, {0,1,0}, {1.0f, 0.0f}},
		{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0,0,-1}, {0,1,0}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0,0,-1}, {0,1,0}, {0.0f, 1.0f}},
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, 2, 3, 0,       // Front
		4, 5, 6, 6, 7, 4,       // Back
		8, 9, 10, 10, 11, 8,    // Top
		12, 13, 14, 14, 15, 12, // Bottom
		16, 17, 18, 18, 19, 16, // Right
		20, 21, 22, 22, 23, 20  // Left
	};

	return assets.Load<Merlin::MeshAsset>("cube", std::move(vertices), std::move(indices));
}

class SampleLayer : public Merlin::Layer {
public:
	SampleLayer() : Layer("SampleLayer") {}

	void OnAttach() override {
		auto& registry = Merlin::Application::Get().GetRegistry();
		auto& assets = Merlin::Application::Get().GetAssets();

		// as of now, assets need to be created manually
		//	and then linked into the component
		auto shader = assets.Load<Merlin::ShaderAsset>(
			"basic",
			"assets/shaders/basic.vert",
			"assets/shaders/basic.frag"
		);
		auto cubeMesh = CreateCubeMesh(assets);

		// create cube entity
		auto cubeEntity = registry.CreateEntity();
		auto& transform = registry.AddComponent<Merlin::Transform>(cubeEntity);
		transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
		transform.rotation = glm::vec3(25.0f, 45.0f, 0.0f);
		transform.scale = glm::vec3(2.0f);

		registry.AddComponent<Merlin::MeshRenderer>(cubeEntity, cubeMesh, shader);

		MERLIN_INFO("Created cube entity with MeshRenderer");
	}

	void OnEvent(Merlin::Event& event) override {
		if (event.IsInCategory(Merlin::EventCategoryKeyboard)) {

			// TODO: need to improve the external API for events
			Merlin::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<Merlin::KeyPressedEvent>([](auto& e) {
				if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
					Merlin::WindowCloseEvent closeEvent;
					Merlin::Application::Get().OnEvent(closeEvent);
				}
				return false;
			});
		}
	}

	void OnUpdate(float deltaTime) override {
		auto& registry = Merlin::Application::Get().GetRegistry();
		auto entities = registry.GetEntitiesWithComponent<Merlin::Transform>();
		for (auto entity : entities) {
			if (registry.HasComponent<Merlin::MeshRenderer>(entity)) {
				auto& transform = registry.GetComponent<Merlin::Transform>(entity);
				transform.rotation.y += 30.0f * deltaTime;
			}
		}
	}
};

MERLIN_APP(
	.WithWindow("Sandbox")
	.AddLayer<SampleLayer>()
	.AddOverlay<Editor>()
)
