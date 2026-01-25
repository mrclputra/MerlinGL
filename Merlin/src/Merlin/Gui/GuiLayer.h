#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include <Merlin/Layer.h>

#include "Widgets/Profiler.h"

namespace Merlin {
	class MERLIN_API GuiLayer : public Layer {
	public:
		GuiLayer();
		~GuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

		void Begin();
		void End();
	private:
		Profiler m_Profiler;
	};
}