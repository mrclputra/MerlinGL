#pragma once

#include <glad/glad.h>

#include <Merlin/Core.h>
#include <Merlin/Logger.h>
#include <Merlin/Events/Event.h>

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application() = default;
        virtual ~Application() = default;

        void Run();
    };

    // this must be defined by the client
    Application* CreateApplication();
};