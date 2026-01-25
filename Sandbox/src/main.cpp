#include <Merlin.h>

using namespace Merlin;

class ExampleLayer : public Merlin::Layer {
public:
	ExampleLayer(const std::string& name) : Layer(name) {}

	void OnUpdate() override {
		if (Input::IsKeyPressed(GLFW_KEY_TAB)) {
			// input poll
			MERLIN_INFO("{0} (poll)", GLFW_KEY_TAB);
		}
	}

	void OnEvent(Event& event) override {
		MERLIN_INFO("{0} | {1}", event.ToString(), this->GetName());

		if (event.GetEventType() == EventType::KeyPressed) {
			KeyPressedEvent& e = (KeyPressedEvent&)event;
			if (e.GetKeyCode() == GLFW_KEY_TAB) {
				// input event
			}
			MERLIN_INFO("{0}", e.GetKeyCode());
		}

		if (event.GetEventType() == EventType::WindowResize) {
			// example of a shared event
		}
		else {
			// example of a non-shared event
			event.handled = true;
		}
	}
};

class Sandbox : public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer("Layer 1"));
		PushLayer(new ExampleLayer("Layer 2"));
		//PushOverlay(new Merlin::GuiLayer());
	}
	~Sandbox() {}
};

MERLIN_APPLICATION(Sandbox)