#pragma once

#include <glad/glad.h>

#include <Merlin/Logger.h>
#include <Merlin/Config.h>

namespace Merlin {
    class MERLIN_API Application {
    public:
        Application() = default;
        virtual ~Application() = default;

        void Run();
    };

    // to be defined by client
    Application* CreateApplication();
};