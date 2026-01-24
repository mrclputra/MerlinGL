#include <Merlin.h>

class ExampleLayer : public Merlin::Layer {
public:
	ExampleLayer(const std::string& name) : Layer(name) {}

	void OnUpdate() override {

	}

	void OnEvent(Merlin::Event& event) override {
		Merlin::logger.info(event.ToString(), " | ", this->GetName());

		if (event.GetEventType() == Merlin::EventType::WindowResize) {
			// example of a shared event
		}
		else {
			// example of a non-shared event
			event.handled = true;
		}
	}
};

class Sandbox : public Merlin::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer("Layer 1"));
		PushLayer(new ExampleLayer("Layer 2"));
	}
	~Sandbox() {}
};

MERLIN_APPLICATION(Sandbox)