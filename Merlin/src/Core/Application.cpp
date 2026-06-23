#include "Application.h"

#include <spdlog/spdlog.h>

using namespace Merlin;

Application::Application(const std::string &title, int width, int height) {
   SPDLOG_INFO("initializing application");

   window = std::make_unique<Window>(title, width, height);
   guiModule = std::make_unique<GuiModule>(window->getNative());
   renderer = std::make_unique<Renderer>(width, height);

   SPDLOG_INFO("application initialized");
}

Application::~Application() {
   quit();
   SPDLOG_INFO("application destroyed");
}

void Application::run() {
   // application loop here
   while (!window->shouldClose()) {
      window->pollEvents();

      renderer->render();

      // glViewport(0, 0, window->getWidth(), window->getHeight());
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      guiModule->Draw(window->getWidth(), window->getHeight(), renderer->framebuffer->colorAttachments[0]);
      renderer->resize(static_cast<int>(guiModule->viewportWidth), static_cast<int>(guiModule->viewportHeight));

      window->swapBuffers();
   }
}

void Application::quit() {
   // cleanup and stuff here
   window->Shutdown();
   glfwTerminate();
}