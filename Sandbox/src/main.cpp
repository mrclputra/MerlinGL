#include <Merlin.h>
#include <Merlin/Core/EntryPoint.h>

#include "Editor.h"

class SampleLayer : public Merlin::Layer {
public:
	SampleLayer() : Layer("SampleLayer") {}

	void OnAttach() override {
		auto& registry = Merlin::Application::Get().GetRegistry();
		auto& assets = Merlin::Application::Get().GetAssets();

		registry.AddSystem<Merlin::RenderSystem>();

		auto shader = assets.Load<Merlin::ShaderAsset>(
			"basic",
			ASSETS_DIR "shaders/basic.vert",
			ASSETS_DIR "shaders/basic.frag"
		);

		//auto meshes = Merlin::ModelLoader::Load(assets, ASSETS_DIR "models/teeth.ply");
		//auto meshes = Merlin::ModelLoader::Load(assets, ASSETS_DIR "models/sibenik/sibenik.obj");
		auto meshes = Merlin::ModelLoader::Load(assets, ASSETS_DIR "models/main_sponza/NewSponza_Main_glTF_003.gltf");

		for (auto& [mesh, material] : meshes) {
			auto entity = registry.CreateEntity();

			auto& transform = registry.AddComponent<Merlin::Transform>(entity);
			//transform.scale = glm::vec3(1.0f);
			transform.scale = glm::vec3(0.1f);
			//transform.rotation = glm::vec3(-90.0f, 0.0f, 0.0f);

			auto& meshRenderer = registry.AddComponent<Merlin::MeshRenderer>(entity, mesh, shader);
			meshRenderer.material = material;
		}
	}

	void OnEvent(Merlin::Event& event) override {
		if (event.IsInCategory(Merlin::EventCategoryKeyboard)) {
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
};

MERLIN_APP(
	.WithWindow("Sandbox")
	.AddLayer<SampleLayer>()
	.AddOverlay<Editor>()
)
