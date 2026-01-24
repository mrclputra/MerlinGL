#pragma once
#include <Merlin/Application.h>

#define MERLIN_APPLICATION(AppClass) \
    Merlin::Application* Merlin::CreateApplication() { \
        return new AppClass(); \
    }

extern Merlin::Application* Merlin::CreateApplication();

int main() {
    auto app = Merlin::CreateApplication();
    app->Run();
    delete app;

    return 0;
}