#include "Application.h"

#include <spdlog/spdlog.h>

using namespace Merlin;

Application::Application(const std::string &title, int width, int height) {
   SPDLOG_INFO("initializing application");

   m_Window = std::make_unique<Window>(title, width, height);
   m_GuiModule = std::make_unique<GuiModule>(m_Window->getNative());

   SPDLOG_INFO("application initialized");
}

Application::~Application() {
   quit();
   SPDLOG_INFO("application destroyed");
}

void Application::run() {
   // application loop here
   while (!m_Window->shouldClose()) {
      m_Window->pollEvents();

      glClearColor(0.1, 0.1, 0.1, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);

      m_GuiModule->Draw(m_Window->getWidth(), m_Window->getHeight());

      m_Window->swapBuffers();
   }
}

void Application::quit() {
   // cleanup and stuff here
   m_Window->Shutdown();
   glfwTerminate();
}
