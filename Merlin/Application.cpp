#include <Merlin/Application.h>

namespace Merlin {
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::make_unique<Window>(WindowProps());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_EventBus.Subscribe<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		m_EventBus.Subscribe<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
	}

	Application::~Application() {}

	void Application::OnEvent(Event& e) {
		m_EventBus.Publish(e);
		if (!e.handled) {
			logger.info(e.ToString());
		}
	}

	void Application::Run() {
		logger.info("MerlinGL Engine is flying!!!");
		while (m_Running) {
			m_Window->OnUpdate();
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