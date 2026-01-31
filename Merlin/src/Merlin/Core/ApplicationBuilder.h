#pragma once

#include "mepch.h"
#include "Core.h"
#include "Application.h"
#include "Layer.h"

namespace Merlin {

	struct MERLIN_API WindowConfig {
		std::string title = "MerlinGL";
		uint32_t width = 1280;
		uint32_t height = 720;
		bool vsync = true;
	};

	class MERLIN_API ApplicationBuilder {
	public:
		ApplicationBuilder() = default;

		// window configuration
		ApplicationBuilder& WithWindow(const WindowConfig& config);
		ApplicationBuilder& WithWindow(const std::string& title, uint32_t width = 1280, uint32_t height = 720);

		// layer registration
		ApplicationBuilder& AddLayer(Layer* layer);
		ApplicationBuilder& AddOverlay(Layer* layer);

		template<typename T, typename... Args>
		ApplicationBuilder& AddLayer(Args&&... args) {
			static_assert(std::is_base_of<Layer, T>::value, "T must derive from Layer");
			return AddLayer(new T(std::forward<Args>(args)...));
		}

		template<typename T, typename... Args>
		ApplicationBuilder& AddOverlay(Args&&... args) {
			static_assert(std::is_base_of<Layer, T>::value, "T must derive from Layer");
			return AddOverlay(new T(std::forward<Args>(args)...));
		}

		// Build the application
		Application* Build();

	private:
		WindowConfig m_WindowConfig;
		std::vector<Layer*> m_Layers;
		std::vector<Layer*> m_Overlays;
	};

}
