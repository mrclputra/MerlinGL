#include "Application.h"

#include <spdlog/spdlog.h>
using namespace Merlin;

Application::Application(const std::string &title, int width, int height) {
   SPDLOG_INFO("initializing application");

   window = std::make_unique<Window>(title, width, height);
   guiModule = std::make_unique<GuiModule>(window->getNative());
   renderer = std::make_unique<Renderer>(width, height);

   // callbacks
   EventBus::get().on<KeyPressedEvent>([this](const KeyPressedEvent& e) {
       auto& vp = renderer->scene.viewports[0];
       if (!vp.focused) return;
       vp.camera->onKeyPress(e.key);
   });
   EventBus::get().on<KeyReleasedEvent>([this](const KeyReleasedEvent& e) {
       auto& vp = renderer->scene.viewports[0];
       vp.camera->onKeyRelease(e.key);
   });
   EventBus::get().on<MouseMovedEvent>([this](const MouseMovedEvent& e) {
       auto& vp = renderer->scene.viewports[0];
       if (!vp.focused) return;
       vp.camera->onMouseMove(e.dx, e.dy);
   });
   EventBus::get().on<MouseScrolledEvent>([this](const MouseScrolledEvent& e) {
       auto& vp = renderer->scene.viewports[0];
       if (!vp.focused) return;
       vp.camera->onScroll(e.delta);
   });

   SPDLOG_INFO("application initialized");
}

Application::~Application() {
   quit();
   SPDLOG_INFO("application destroyed");
}

void Application::run() {
   // application loop here
   while (!window->shouldClose()) {
      static float lastTime = 0.0f;
      float now = static_cast<float>(glfwGetTime());
      float delta = now - lastTime;
      lastTime = now;

      auto &vp = renderer->scene.viewports.front();
      // bool wasFocused = vp.focused;
      vp.focused = guiModule->viewportFocused;
      vp.camera->update(delta);

      glfwSetInputMode(window->getNative(), GLFW_CURSOR, vp.focused ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

      window->pollEvents();
      renderer->render();

      // glViewport(0, 0, window->getWidth(), window->getHeight());
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      guiModule->Draw(window->getWidth(), window->getHeight(), renderer->scene.viewports.front().framebuffer->colorAttachments[0]);
      renderer->resize(static_cast<int>(guiModule->viewportWidth), static_cast<int>(guiModule->viewportHeight));

      window->swapBuffers();
   }
}

void Application::quit() {
   // cleanup and stuff here
   window->Shutdown();
   glfwTerminate();
}