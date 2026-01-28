#pragma once

#include "mepch.h"

#include "Core.h"
#include "Logger.h"
#include "LayerStack.h"
#include "Platform/GLFW/Window.h"
#include "Gui/GuiModule.h"

#include "Events/Event.h"
#include "Events/AppEvents.h"

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        LayerStack m_LayerStack;
        // lets integrate gui straight into this engine

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<GuiModule> m_GuiModule;
        bool m_Running = true;

    private:
        static Application* s_Instance;
    };

    // this must be defined by the client
    Application* CreateApplication();
};