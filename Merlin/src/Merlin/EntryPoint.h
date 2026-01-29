#pragma once
#include "Application.h"
#include "ApplicationBuilder.h"

// define application using builder pattern
// Usage: MERLIN_APP(.WithWindow("Title").AddLayer<MyLayer>().AddOverlay<Editor>())
#define MERLIN_APP(BuilderCode) \
    Merlin::Application* Merlin::CreateApplication() { \
        return Merlin::ApplicationBuilder() BuilderCode .Build(); \
    }

// todo: consider expanding API to include Renderer context management?
//  or maybe we should implement that within the layer only?

extern Merlin::Application* Merlin::CreateApplication();

int main() {
    auto app = Merlin::CreateApplication();
    app->Run();
    delete app;

    return 0;
}