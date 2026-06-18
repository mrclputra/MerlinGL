#include "GuiModule.h"

#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Merlin
{
    GuiModule::GuiModule(void* native_window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = nullptr;

        auto* window = static_cast<GLFWwindow*>(native_window);
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

    void GuiModule::BeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GuiModule::EndFrame(const unsigned int width, const unsigned int height) {
        ImGuiIO& io  = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // namespace Merlin