#include "mepch.h"
#include "ApplicationBuilder.h"

namespace Merlin {

	ApplicationBuilder& ApplicationBuilder::WithWindow(const WindowConfig& config) {
		m_WindowConfig = config;
		return *this;
	}

	ApplicationBuilder& ApplicationBuilder::WithWindow(const std::string& title, uint32_t width, uint32_t height) {
		m_WindowConfig.title = title;
		m_WindowConfig.width = width;
		m_WindowConfig.height = height;
		return *this;
	}

	ApplicationBuilder& ApplicationBuilder::AddLayer(Layer* layer) {
		m_Layers.push_back(layer);
		return *this;
	}

	ApplicationBuilder& ApplicationBuilder::AddOverlay(Layer* layer) {
		m_Overlays.push_back(layer);
		return *this;
	}

	Application* ApplicationBuilder::Build() {
		WindowProps props(m_WindowConfig.title, m_WindowConfig.width, m_WindowConfig.height);
		Application* app = new Application(props);

		for (Layer* layer : m_Layers) {
			app->PushLayer(layer);
		}

		for (Layer* overlay : m_Overlays) {
			app->PushOverlay(overlay);
		}

		app->GetWindow().SetVSync(m_WindowConfig.vsync);

		return app;
	}

}
