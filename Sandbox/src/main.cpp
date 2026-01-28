#include <Merlin.h>
#include <Merlin/EntryPoint.h>

#include <Merlin/Gui/Widgets/Console.h>
#include <Merlin/Gui/Widgets/Profiler.h>

#include "Editor.h"

class ExampleLayer : public Merlin::Layer {
public:
	ExampleLayer(const std::string& name) : Layer(name) {}

	void OnUpdate() override {
		if (Merlin::Input::IsKeyPressed(GLFW_KEY_TAB)) {
			// input poll
			MERLIN_INFO("{0} polled by {1}", GLFW_KEY_TAB, this->GetName());
		}
	}

	void OnEvent(Merlin::Event& event) override {
		if (event.IsInCategory(Merlin::EventCategoryApplication)) return;
		if (event.GetEventType() == Merlin::EventType::MouseMoved) return;

		MERLIN_INFO("{0} from {1}", event.ToString(), this->GetName());
		event.handled = true;
	}
};

class Sandbox : public Merlin::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer("Layer 1"));
		PushLayer(new ExampleLayer("Layer 2"));
		PushOverlay(new Editor());
	}
	~Sandbox() {}
};

MERLIN_APPLICATION(Sandbox)