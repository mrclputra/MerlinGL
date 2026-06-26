#include "GUI/GuiModule.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

namespace Merlin {
GuiModule::GuiModule(void *native_window) {
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();

   ImGuiIO &io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
   io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
   io.IniFilename = nullptr;

   auto *window = static_cast<GLFWwindow *>(native_window);
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 460");

   // get logger
   for (auto &sink : spdlog::default_logger()->sinks()) {
      if (auto rb = std::dynamic_pointer_cast<spdlog::sinks::ringbuffer_sink_mt>(sink)) {
         ringSink = rb;
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

void GuiModule::Draw(uint32_t sceneTexture) {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();  // sets io.DisplaySize from GLFW
   ImGui::NewFrame();

   ImGuiIO &io = ImGui::GetIO();

   // opengl viewport
   // todo: in the future implement gui class modules like QT
   ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   ImGui::SetNextWindowPos(ImVec2(0, 0));
   ImGui::SetNextWindowSize(io.DisplaySize);
   ImGui::Begin("Viewport", nullptr,
                ImGuiWindowFlags_NoDecoration |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);
   ImVec2 size = ImGui::GetContentRegionAvail();
   viewportWidth = size.x;
   viewportHeight = size.y;
   ImGui::Image(sceneTexture, size, ImVec2(0, 1), ImVec2(1, 0));
   if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
      viewportFocused = true;
   if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
      viewportFocused = false;
   ImGui::End();
   ImGui::PopStyleVar(2);

   // logger
   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   ImGui::SetNextWindowPos({10, 10}, ImGuiCond_Always);
   ImGui::SetNextWindowBgAlpha(0.0f);
   ImGui::Begin("Log", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);
   for (auto &msg : ringSink->last_formatted())
      ImGui::TextUnformatted(msg.c_str());
   ImGui::End();
   ImGui::PopStyleVar();
   // end draw calls

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}  // namespace Merlin