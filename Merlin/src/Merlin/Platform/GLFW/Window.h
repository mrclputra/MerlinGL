#pragma once

#include "mepch.h"
#include "Merlin/Core.h"
#include "Merlin/Events/Event.h"

// might turn this class into a template further down the line in development
// this is so that we can implement different window APIs instead of just GLFW such as SDL
// but need to make sure all features are cross-compatible first; such as a mapping for keycodes and inputs to events

namespace Merlin {
    struct WindowProps {
        std::string title;
        unsigned int width, height;
        WindowProps(const std::string& title = "MerlinGL", uint32_t width = 1280, uint32_t height = 720)
            : title(title), width(width), height(height) {}
    };

    class MERLIN_API Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props);
        ~Window();

        void OnUpdate();
        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
        void SetVSync(bool enabled);
        inline bool IsVSync() const;

        inline GLFWwindow* GetNativeWindow() const { return m_Window; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;

        std::string m_Title;
        uint32_t m_Width, m_Height;
        bool m_VSync;

        EventCallbackFn m_EventCallback;
    };
}