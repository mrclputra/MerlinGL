#pragma once

#include "mepch.h"

#include "Core.h"
#include "Logger.h"
#include "LayerStack.h"
#include "Platform/GLFW/Window.h"
#include "Gui/GuiModule.h"

#include "Events/Event.h"
#include "Events/AppEvents.h"
#include "ECS/Registry.h"
#include "Assets/AssetManager.h"

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application();
        Application(const WindowProps& windowProps);
        virtual ~Application();

        void Run();
        void Quit() { m_Running = false; }
        void OnEvent(Event& event);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline static Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }
        inline Registry& GetRegistry() { return *m_Registry; }
        inline AssetManager& GetAssets() { return *m_Assets; }

    private:
        void Init(const WindowProps& windowProps);
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        LayerStack m_LayerStack;

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<GuiModule> m_GuiModule;
        std::unique_ptr<Registry> m_Registry;
        std::unique_ptr<AssetManager> m_Assets;
        bool m_Running = true;

    private:
        static Application* s_Instance;
    };

    // this must be defined by the client
    Application* CreateApplication();
};