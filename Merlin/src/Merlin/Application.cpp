#include <Merlin/Application.h>

namespace Merlin {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;
		m_Window = std::make_unique<Window>(WindowProps());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// define built-in layers here
		m_GuiLayer = new GuiLayer();
		PushOverlay(m_GuiLayer);

		m_EventBus.Subscribe<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		m_EventBus.Subscribe<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
	}

	Application::~Application() {}

	void Application::Run() {
		logger.info("MerlinGL Engine is flying!!!");
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& event) {
		m_EventBus.Publish(event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.handled)
				break;
		}
	}

	// callback functions
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e) {
		if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
			m_Running = false;
			return true;
		}
		return false;
	}
}