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

	Application* ApplicationBuilder::Build() {
		WindowProps props(m_WindowConfig.title, m_WindowConfig.width, m_WindowConfig.height);
		Application* app = new Application(props);

		for (auto& factory : m_SystemFactories) {
			factory(app->GetRegistry());
		}

		app->GetWindow().SetVSync(m_WindowConfig.vsync);

		return app;
	}

}
