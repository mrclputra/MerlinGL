#include "GuiModule.h"

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Merlin {
GuiModule::GuiModule(void *native_window) {
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();

   ImGuiIO &io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   io.IniFilename = nullptr;

   auto *window = static_cast<GLFWwindow *>(native_window);
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 460");

   SPDLOG_INFO("IMGUI initialized");
}

GuiModule::~GuiModule() {
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   SPDLOG_INFO("IMGUI destroyed");
}

ImGuiContext *GuiModule::GetContext() {
   return ImGui::GetCurrentContext();
}

void GuiModule::Draw(const unsigned int windowWidth, const unsigned int windowHeight) {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   ImGuiIO &io = ImGui::GetIO();
   io.DisplaySize.x = static_cast<float>(windowWidth);
   io.DisplaySize.y = static_cast<float>(windowHeight);

   // todo: add imgui drawcalls here
   ImGui::ShowDemoWindow();
   // end draw calls

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace Merlin
