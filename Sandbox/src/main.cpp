#include <Merlin.h>

using namespace Merlin;

class ExampleLayer : public Merlin::Layer {
public:
	ExampleLayer(const std::string& name) : Layer(name) {}

	void OnUpdate() override {
		if (Input::IsKeyPressed(GLFW_KEY_TAB)) {
			// input poll
			MERLIN_INFO("{0} polled by {1}", GLFW_KEY_TAB, this->GetName());
		}
	}

	void OnEvent(Event& event) override {
		if (event.IsInCategory(EventCategoryApplication)) return;
		if (event.GetEventType() == EventType::MouseMoved) return;

		MERLIN_INFO("{0} from {1}", event.ToString(), this->GetName());
		event.handled = true;

		//if (event.GetEventType() == EventType::KeyPressed) {
		//	KeyPressedEvent& e = (KeyPressedEvent&)event;
		//	if (e.GetKeyCode() == GLFW_KEY_TAB) {
		//		// input event
		//	}

		//	if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
		//		// this is how we can dispatch events into the internal engine
		//		// note that this is layer-specific

		//		//WindowCloseEvent closeEvent;
		//		//Application::Get().OnEvent(closeEvent);
		//	}
		//}

		//if (event.GetEventType() == EventType::WindowResize) {
		//	// example of a shared event
		//}
		//else {
		//	// example of a non-shared event
		//	event.handled = true;
		//}
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