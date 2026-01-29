#include "mepch.h"
#include "GuiModule.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Style.h"

namespace Merlin {
	GuiModule::GuiModule(void* nativeWindow) : m_NativeWindow(nativeWindow) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.IniFilename = nullptr;

		Style::ApplyTheme();

		GLFWwindow* window = static_cast<GLFWwindow*>(m_NativeWindow);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void GuiModule::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	ImGuiContext* GuiModule::GetContext() {
		return ImGui::GetCurrentContext();
	}

	void GuiModule::BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiModule::EndFrame(uint32_t width, uint32_t height) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)width, (float)height);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GuiModule::OnEvent(Event& event) {
		ImGuiIO& io = ImGui::GetIO();

		if (event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse)
			event.handled = true;
		if (event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard)
			event.handled = true;
	}
}
