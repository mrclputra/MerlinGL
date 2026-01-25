#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuiFileDialog.h>

#include <Merlin/Layer.h>

namespace Merlin {
	class MERLIN_API GuiLayer : public Layer {
	public:
		GuiLayer() :Layer("Gui Layer") {}

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate() override;
		void Begin();
		void End();

		void OnEvent(Event& event) override;
	private:
		float m_Time = 0.0f;
	};
}