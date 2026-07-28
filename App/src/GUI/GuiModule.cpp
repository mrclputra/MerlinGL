#include "GUI/GuiModule.h"
#include "EventBus.h"
#include "Events.h"
#include "Scene.h"
#include "gui_pch.h"

#include <imgui_internal.h> // dockbuilder;

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

void GuiModule::Draw(Scene &scene) {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();  // sets io.DisplaySize from GLFW
   ImGui::NewFrame();
   ImGuiID dockspaceID = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

   static bool firstFrame = true;
   if (firstFrame && !scene.viewports.empty()) {
      firstFrame = false;
      ImGui::DockBuilderDockWindow("Viewport 0", dockspaceID); // note that this name is pre-assigned
      ImGui::DockBuilderFinish(dockspaceID);
   }

   ImGuiIO &io = ImGui::GetIO();

   // opengl viewports
   // todo: in the future implement gui class modules like QT
   ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
   int pendingDelete = -1;
   for (unsigned int i = 0; i < scene.viewports.size(); ++i) { // currently we just automatically build opengl windows for the viewports
      auto &vp = scene.viewports[i];
      std::string label = "Viewport " + std::to_string(i);
      bool open = true;
      ImGui::Begin(label.c_str(), &open);

      ImVec2 size = ImGui::GetContentRegionAvail();
      if (size.x > 0 && size.y > 0 &&
          (static_cast<int>(size.x) != static_cast<int>(vp.width) ||
           static_cast<int>(size.y) != static_cast<int>(vp.height))) {
         EventBus::get().emit(ViewportResizeEvent{i, static_cast<int>(size.x), static_cast<int>(size.y)});
      }

      ImGui::Image(vp.framebuffer->colorAttachments[0], size, ImVec2(0, 1), ImVec2(1, 0));

      vp.hovered = ImGui::IsWindowHovered();
      if (vp.hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
         for (auto &other : scene.viewports)
            other.focused = false;
         vp.focused = true;
      }
      if (vp.focused && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
         vp.focused = false;

      ImGui::End();

      if (!open)
         pendingDelete = static_cast<int>(i);
   }
   ImGui::PopStyleVar();

   if (pendingDelete >= 0) {
      EventBus::get().emit(DeleteViewportEvent{static_cast<size_t>(pendingDelete)});
   }

   // logger
   ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
   ImGui::SetNextWindowPos({10, io.DisplaySize.y - 10}, ImGuiCond_Always, {0.0f, 1.0f});
   ImGui::SetNextWindowBgAlpha(0.0f);
   ImGui::Begin("Log", nullptr,
      ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_NoInputs |
      ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoNav);
   ImGui::SetWindowFontScale(0.82f);
   for (auto &msg : ringSink->last_formatted())
      ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.7f), "%s", msg.c_str());
   ImGui::SetWindowFontScale(1.0f);
   ImGui::End();
   ImGui::PopStyleVar();

   // config
   ImGui::SetNextWindowPos({10, 50}, ImGuiCond_Once);
   ImGui::Begin("Config", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

   ImGui::Text("MerlinGL, WIP renderer");
   ImGui::Separator();
   if (ImGui::Button("load from file")) {
      SPDLOG_INFO("load from file button pressed");
      if (const auto results = pfd::open_file("Load Model", ".", {"3D Models", "*.gltf *.glb *.ply *.obj", "All Files", "*"}).result(); !results.empty()) {
         EventBus::get().emit(LoadModelEvent{results[0]});
      } else {
         SPDLOG_INFO("no file was selected");
      }
   }
   if (ImGui::Button("create viewport")) {
      EventBus::get().emit(CreateViewportEvent{400, 400});
   }

   if (ImGui::Button("reload shaders")) {
      SPDLOG_INFO("reload shader button pressed");
      EventBus::get().emit(ReloadShadersEvent{});
   }

   // // todo: implement some kind camera imgui builder serialization
   // ImGui::Separator();
   // if (ImGui::Button("reset cameras"))

   ImGui::End();

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}  // namespace Merlin