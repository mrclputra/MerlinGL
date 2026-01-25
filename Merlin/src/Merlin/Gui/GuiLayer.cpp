
#include "mepch.h"
#include "GuiLayer.h"

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

        ImGui::Begin("Debug");
        ImGui::Text("Hello Merlin Engine!");
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

        if (event.GetEventType() == EventType::KeyPressed) {
            // this is a niche case, but we can do this if we want this layer to ignore certain keys
            // this is implemented on engine side, which is bad but I did anyways due to the guilayer being "forced"
            auto& e = (KeyPressedEvent&)event;
            if (e.GetKeyCode() == GLFW_KEY_ESCAPE) return; // fall-through
        }

        if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
            if (event.GetEventType() == EventType::KeyPressed) {
                auto& e = (KeyPressedEvent&)event;
                MERLIN_CORE_INFO("imgui {0}", event.ToString());
            }
            if (event.GetEventType() == EventType::MouseButtonPressed) {
                auto& e = (MouseButtonPressedEvent&)event;
                MERLIN_CORE_INFO("imgui {0}", event.ToString());
            }
            event.handled = true;
        }
    }
}