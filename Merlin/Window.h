#pragma once

#include "mepch.h"
#include <Merlin/Core.h>
#include <Merlin/Events/Event.h>

namespace Merlin {
    struct WindowProps {
        std::string title;
        unsigned int width, height;
        WindowProps(const std::string& t = "MerlinGL", uint32_t w = 640, uint32_t h = 480)
            : title(t), width(w), height(h) {}
    };

    class MERLIN_API Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props);
        ~Window();

        void OnUpdate();
        uint32_t GetWidth() const { return m_Data.Width; }
        uint32_t GetHeight() const { return m_Data.Height; }

        void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled);
        bool IsVSync() const { return m_Data.VSync; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        GLFWwindow* m_Window;
        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };
}