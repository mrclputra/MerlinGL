#include "GuiLayer.h"
#include <Merlin/Application.h>

namespace Merlin {
    void GuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
    }

    void GuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GuiLayer::OnUpdate() {
        Begin();

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
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    // handle gui events
    void GuiLayer::OnEvent(Event& event) {
        ImGuiIO& io = ImGui::GetIO();

        if (io.WantCaptureMouse) {
            if (event.GetEventType() == EventType::MouseButtonPressed || event.GetEventType() == EventType::MouseButtonReleased) {
                logger.info("imgui ", event.ToString());
            }
            event.handled = true;
        }
        if (io.WantCaptureKeyboard) {
            logger.info("imgui ", event.ToString());
            event.handled = true;
        }
    }
}