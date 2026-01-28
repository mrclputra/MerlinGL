#pragma once

#include <Merlin/Events/Event.h>

#include <imgui.h>

namespace Merlin {
	class MERLIN_API GuiModule {
	public:
		GuiModule() { Init(); }
		~GuiModule() { Shutdown(); }
		
		struct ImGuiContext* GetContext();

		void Init();
		void Shutdown();

		void BeginFrame();
		void EndFrame();

		void OnEvent(Event& event);
	};
}