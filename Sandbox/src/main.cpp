#include <Merlin.h>
#include <Merlin/Core/EntryPoint.h>

#include "Editor.h"

class SampleSystem : public Merlin::System {
public:
	SampleSystem() : System("SampleSystem") {}

	void OnAttach() override {
		auto& assets = Merlin::Application::Get().GetAssets();

		m_Registry->AddSystem<Merlin::RenderSystem>();

		auto shader = assets.Load<Merlin::ShaderAsset>(
			"basic",
			ASSETS_DIR "shaders/basic.vert",
			ASSETS_DIR "shaders/basic.frag"
		);

		auto meshes = Merlin::ModelLoader::Load(assets, ASSETS_DIR "models/teeth.ply");

		for (auto& [mesh, material] : meshes) {
			auto entity = m_Registry->CreateEntity();

			auto& transform = m_Registry->AddComponent<Merlin::Transform>(entity);
			transform.scale = glm::vec3(0.1f);

			auto& meshRenderer = m_Registry->AddComponent<Merlin::MeshRenderer>(entity, mesh, shader);
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
	.AddSystem<SampleSystem>()
	.AddSystem<EditorSystem>()
)
