#include "GUI/GuiModule.h"
#include "Core/EventBus.h"
#include "Core/Events.h"
#include "Engine/Scene.h"

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
   ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

   ImGuiIO &io = ImGui::GetIO();

   // opengl viewports
   // todo: in the future implement gui class modules like QT
   ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
   for (unsigned int i = 0; i < scene.viewports.size(); ++i) { // currently we just automatically build opengl windows for the viewports
      auto &vp = scene.viewports[i];
      std::string label = "Viewport " + std::to_string(i);
      ImGui::Begin(label.c_str());

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
   }
   ImGui::PopStyleVar();

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

   // TODO: load button should open the windows file api
   //    -> pass resulting string to the loader and update registry

   ImGui::Text("Welcome to my rendering engine");
   if (ImGui::Button("load mesh")) {
      SPDLOG_INFO("load mesh file button pressed");
      if (const auto results = pfd::open_file("Load Mesh", ".", {"3D Models", "*.gltf *.glb *.ply *.obj", "All Files", "*"}).result(); !results.empty()) {
         EventBus::get().emit(LoadMeshEvent{results[0]});
      } else {
         SPDLOG_INFO("no file selected");
      }
   }
   if (ImGui::Button("load point cloud")) {
      SPDLOG_INFO("load point cloud button pressed");
      if (const auto results = pfd::open_file("Load Point Cloud", ".", {"3D Models", "*.gltf *.glb *.ply *.obj", "All Files", "*"}).result(); !results.empty()) {
         EventBus::get().emit(LoadPCDEvent{results[0]});
      } else {
         SPDLOG_INFO("no file selected");
      }
   }
   if (ImGui::Button("create viewport")) {
      EventBus::get().emit(CreateViewportEvent{400, 400});
   }

   if (ImGui::Button("reload shaders")) {
      SPDLOG_INFO("reload shader button pressed");
      EventBus::get().emit(ReloadShadersEvent{});
   }

   ImGui::End();

   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}  // namespace Merlin