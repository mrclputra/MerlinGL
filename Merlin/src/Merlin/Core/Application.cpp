#include "Application.h"
#include "Merlin/Events/EventBus.h"
#include "Input.h"

#include "Merlin/Assets/ShaderAsset.h"

namespace Merlin {
	Application* Application::s_Instance = nullptr;

	Application::Application() {
		Init(WindowProps());
	}

	Application::Application(const WindowProps& windowProps) {
		Init(windowProps);
	}

	void Application::Init(const WindowProps& windowProps) {
		s_Instance = this;
		m_Window = std::make_unique<Window>(windowProps);
		m_Window->SetEventCallback(MERLIN_BIND_FN(OnEvent));

		Input::Init(m_Window->GetNativeWindow());
		m_GuiModule = std::make_unique<GuiModule>(m_Window->GetNativeWindow());

		m_Registry = std::make_unique<Registry>();
		m_Assets = std::make_unique<AssetManager>();

		glEnable(GL_DEPTH_TEST);
	}

	Application::~Application() {}

	void Application::Run() {
		MERLIN_CORE_INFO("Merlin Engine is flying!!!");

		while (m_Running) {
			/*AppUpdateEvent updateEvent;
			OnEvent(updateEvent);*/

			// Clear main framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
			glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// update ECS systems
			m_Registry->Update(0.016f); // TODO: calculate actual delta time

			m_Assets->ForEach<ShaderAsset>([](auto& shader) {
				shader->checkHotReload();
				});

			// standard frames
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(0.016f);

			// render pass
			for (Layer* layer : m_LayerStack)
				layer->OnRender();

			// GUI Frames
			m_GuiModule->BeginFrame();
			for (Layer* layer : m_LayerStack)
				layer->OnGuiRender();
			m_GuiModule->EndFrame(m_Window->GetWidth(), m_Window->GetHeight());

			/*AppRenderEvent renderEvent;
			OnEvent(renderEvent);*/

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event) {
		EventBus::Get().Emit(event);
		EventDispatcher dispatcher(event);

		// system events, these are if we want to hardcode functionality to certain events
		dispatcher.Dispatch<WindowCloseEvent>(MERLIN_BIND_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(MERLIN_BIND_FN(OnWindowResize));

		// let gui capture events first
		m_GuiModule->OnEvent(event);
		if (event.handled) return;

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(event);
			if (event.handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
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