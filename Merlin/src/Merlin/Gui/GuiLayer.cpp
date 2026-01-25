
#include "mepch.h"
#include "GuiLayer.h"
#include "Widgets/Profiler.h"

#include <Merlin/Application.h>
#include <Merlin/Events/InputEvents.h>

namespace Merlin {
    GuiLayer::GuiLayer() : Layer("Gui Layer") {}
    GuiLayer::~GuiLayer() {}

    void GuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.IniFilename = nullptr;

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        ImGui::SetNextWindowSize(ImVec2(100, 200));
    }

    void GuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiLayer::OnUpdate() {
        Begin();

        ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), 
            ImGuiDockNodeFlags_PassthruCentralNode | 
            ImGuiDockNodeFlags_NoTabBar
        );

        static bool first_run = true;
        if (first_run) {
            first_run = false;

            ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
            ImGui::DockBuilderDockWindow("Debug", dock_left_id);
            ImGui::DockBuilderFinish(dockspace_id);
        }


        // ui
        ImGui::Begin("Debug");
        ImGui::Text("Hello Merlin Engine!");
        ImGui::Separator();
        m_Profiler.OnImGuiRender();
        ImGui::End();

        End();
    }

    void GuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GuiLayer::OnEvent(Event& event) {
        ImGuiIO& io = ImGui::GetIO();

        if (event.IsInCategory(EventCategoryMouse) && io.WantCaptureMouse) {
            MERLIN_CORE_INFO("imgui {0}", event.ToString());
            event.handled = true;
        }

        if (event.IsInCategory(EventCategoryKeyboard) && io.WantCaptureKeyboard) {
            MERLIN_CORE_INFO("imgui {0}", event.ToString());
            event.handled = true;
        }
    }
}