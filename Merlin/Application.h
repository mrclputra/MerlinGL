#pragma once

#include "mepch.h"

#include <Merlin/Core.h>
#include <Merlin/Logger.h>
#include <Merlin/Window.h>

#include <Merlin/Events/Event.h>
#include <Merlin/Events/AppEvents.h>
#include <Merlin/Events/InputEvents.h>

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Event& e);
        void Run();

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);

        std::unique_ptr<Window> m_Window;
        EventBus m_EventBus;
        bool m_Running = true;
    };

    // this must be defined by the client
    Application* CreateApplication();
};