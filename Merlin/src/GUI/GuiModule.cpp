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

   // get logger
   for (auto& sink : spdlog::default_logger()->sinks()) {
      if (auto rb = std::dynamic_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(sink)) {
         m_RingSink = rb;
         break;
      }
   }

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
   // ImGui::ShowDemoWindow();

   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Always);
   ImGui::SetNextWindowBgAlpha(0.0f);
   ImGui::Begin("##log", nullptr,
      ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_NoInputs |
      ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoNav
   );
   for (auto& msg : m_RingSink->last_formatted())
      ImGui::TextUnformatted(msg.c_str());
   ImGui::End();
   ImGui::PopStyleVar();
   // end draw calls

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
} // namespace Merlin
