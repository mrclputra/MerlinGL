#pragma once

#include "mepch.h"

// TODO: expose libraries into a singular functional API
#include <Merlin/Core.h>
#include <Merlin/Logger.h>
#include <Merlin/Window.h>
#include <Merlin/LayerStack.h>

#include <Merlin/Events/Event.h>
#include <Merlin/Events/AppEvents.h>
#include <Merlin/Events/InputEvents.h>

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);

        LayerStack m_LayerStack;
        std::unique_ptr<Window> m_Window;
        EventBus m_EventBus;
        bool m_Running = true;
    };

    // this must be defined by the client
    Application* CreateApplication();
};