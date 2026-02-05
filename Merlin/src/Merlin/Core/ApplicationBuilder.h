#pragma once

#include "mepch.h"
#include "Core.h"
#include "Application.h"
#include "Merlin/ECS/System.h"
#include <functional>

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

		ApplicationBuilder& WithWindow(const WindowConfig& config);
		ApplicationBuilder& WithWindow(const std::string& title, uint32_t width = 1280, uint32_t height = 720);

		template<typename T, typename... Args>
		ApplicationBuilder& AddSystem(Args&&... args) {
			static_assert(std::is_base_of<System, T>::value, "T must derive from System");
			m_SystemFactories.push_back([args = std::make_tuple(std::forward<Args>(args)...)](Registry& registry) mutable {
				std::apply([&registry](auto&&... a) {
					registry.AddSystem<T>(std::forward<decltype(a)>(a)...);
				}, std::move(args));
			});
			return *this;
		}

		Application* Build();

	private:
		WindowConfig m_WindowConfig;
		std::vector<std::function<void(Registry&)>> m_SystemFactories;
	};

}
