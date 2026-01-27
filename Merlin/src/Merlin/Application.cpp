#include <Merlin/Application.h>

namespace Merlin {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;
		m_Window = std::make_unique<Window>(WindowProps());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// define built-in layers here

		//Logger::getClientLogger().SetCallback([](const std::string& msg) {
		//	Console::AddLog(msg);
		//});
	}

	Application::~Application() {}

	void Application::Run() {
		MERLIN_CORE_INFO("Merlin Engine is flying!!!");

		while (m_Running) {
			AppUpdateEvent updateEvent;
			OnEvent(updateEvent);

			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			AppRenderEvent renderEvent;
			OnEvent(renderEvent);

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
		EventDispatcher dispatcher(event);

		// system events, these are if we want to hardcode functionality to certain events
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

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

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		return false;
	}
}