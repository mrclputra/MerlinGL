#include <Merlin.h>
#include <Merlin/EntryPoint.h>
#include <Merlin/ECS/Registry.h>

#include "Editor.h"

// simple component
struct SimpleComponent : public Merlin::Component {
	COMPONENT_TYPE(SimpleComponent)
	std::string Name;
	SimpleComponent(const std::string& name) : Name(name) {}
};

// simple system (registry is auto-injected)
class SampleSystem : public Merlin::System {
public:
	SampleSystem() : System("SampleSystem") {}

	void OnAttach() override {
		MERLIN_INFO("SampleSystem attached");
	}

	void OnUpdate(float deltaTime) override {
		// m_Registry is automatically available
		if (m_Registry->HasComponent<SimpleComponent>(1)) {
			auto& comp = m_Registry->GetComponent<SimpleComponent>(1);
		}
	}
};

class SampleLayer : public Merlin::Layer {
public:
	SampleLayer() : Layer("SampleLayer") {}

	void OnAttach() override {
		auto& registry = Merlin::Application::Get().GetRegistry();

		// create entity, with component
		auto entity = registry.CreateEntity();
		registry.AddComponent<SimpleComponent>(entity, "Player");

		// add system
		registry.AddSystem<SampleSystem>();

		MERLIN_INFO("Created entity {0} with name: {1}",
			entity.GetID(),
			registry.GetComponent<SimpleComponent>(entity).Name);
	}

	void OnEvent(Merlin::Event& event) override {
		if (event.IsInCategory(Merlin::EventCategoryKeyboard)) {
			MERLIN_INFO(event.ToString());
		}
	}

	void OnUpdate(float deltaTime) override {
		if (Merlin::Input::IsKeyPressed(GLFW_KEY_TAB)) {
			// do something
			// this is an example of input polling
		}
	}
};

MERLIN_APP(
	.WithWindow("Sandbox")
	.AddLayer<SampleLayer>()
	.AddOverlay<Editor>()
)
