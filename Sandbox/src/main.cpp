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
	}
};

class Editor : public Layer {
public:
	Editor() : Layer("Editor") {}

	void OnGuiRender() override {
		// menu bar
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// todo: call event to exit app
				ImGui::MenuItem("Exit", nullptr, nullptr, false); 
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		// build dockspace
		ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
			ImGuiDockNodeFlags_PassthruCentralNode |
			ImGuiDockNodeFlags_NoTabBar
		);

		ImGui::Begin("Debug");
		ImGui::Text("Hello Sandbox!!!");
		ImGui::End();
	}
};

class Sandbox : public Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer("Layer 1"));
		PushLayer(new ExampleLayer("Layer 2"));
		PushOverlay(new Editor());
	}
	~Sandbox() {}
};

MERLIN_APPLICATION(Sandbox)