#include <Merlin.h>
#include <Merlin/EntryPoint.h>

#include "Editor.h"

class ExampleLayer : public Merlin::Layer {
public:
	ExampleLayer(const std::string& name) : Layer(name) {}

	void OnUpdate() override {
		if (Merlin::Input::IsKeyPressed(GLFW_KEY_TAB)) {
			MERLIN_INFO("{0} polled by {1}", GLFW_KEY_TAB, this->GetName());
		}
	}

	void OnEvent(Merlin::Event& event) override {
		if (event.IsApplication()) return;
		if (event.GetEventType() == Merlin::EventType::MouseMoved) return;

		MERLIN_INFO("{0} from {1}", event.ToString(), this->GetName());
		event.handled = true;
	}
};

// this is an example of how to initialize the Engine API
MERLIN_APP(
	.WithWindow("Sandbox")
	.AddLayer<ExampleLayer>("Layer 1")
	.AddLayer<ExampleLayer>("Layer 2")
	.AddOverlay<Editor>()
)