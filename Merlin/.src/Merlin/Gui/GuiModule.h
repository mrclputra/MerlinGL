#pragma once

#include "Merlin/Core/Core.h"
#include "Merlin/Events/Event.h"

#include <imgui.h>

namespace Merlin {
	class MERLIN_API GuiModule {
	public:
		GuiModule(void* nativeWindow);
		~GuiModule() { Shutdown(); }

		struct ImGuiContext* GetContext();

		void Shutdown();

		void BeginFrame();
		void EndFrame(uint32_t width, uint32_t height);

		void OnEvent(Event& event);

	private:
		void* m_NativeWindow = nullptr;
	};
}