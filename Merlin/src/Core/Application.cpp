#include "Application.h"

#include <spdlog/spdlog.h>

using namespace Merlin;

Application::Application() {
   SPDLOG_INFO("initializing application");

   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(1280, 720, "MerlinGL", nullptr, nullptr);
   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   guiModule = std::make_unique<GuiModule>(window);

   SPDLOG_INFO("application initialized");
}

Application::~Application() {
   quit();
   SPDLOG_INFO("application destroyed");
}

void Application::run() {
   // application loop here
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      glClearColor(0.1, 0.1, 0.1, 1.0);
      glClear(GL_COLOR_BUFFER_BIT);

      GuiModule::BeginFrame();

      int width, height;
      glfwGetWindowSize(window, &width,
                        &height); // this is bad, we need that window class
      guiModule->EndFrame(width, height);

      glfwSwapBuffers(
         window); // todo: move window management to it's own class/module
   }
}

void Application::quit() {
   // cleanup and stuff here
   glfwDestroyWindow(window);
   glfwTerminate();
}
