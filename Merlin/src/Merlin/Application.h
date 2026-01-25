#pragma once

#include "mepch.h"

// TODO: expose libraries into a singular functional API
#include <Merlin/Core.h>
#include <Merlin/Logger.h>
#include <Merlin/LayerStack.h>

#include <Merlin/Gui/GuiLayer.h>
#include <Merlin/Platform/GLFW/Window.h>

#include <Merlin/Events/Event.h>
#include <Merlin/Events/AppEvents.h>

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

        // public APIs for the eventbus
        template<typename T>
        void Subscribe(EventBus::EventCallback<T> callback) {
            m_EventBus.Subscribe<T>(callback);
        }
        void Dispatch(Event& e) { OnEvent(e); }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        LayerStack m_LayerStack;
        std::unique_ptr<Window> m_Window;
        EventBus m_EventBus;
        bool m_Running = true;

    private:
        static Application* s_Instance;
        GuiLayer* m_GuiLayer;
    };

    // this must be defined by the client
    Application* CreateApplication();
};