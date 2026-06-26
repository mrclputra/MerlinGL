#include "Core/Application.h"
#include "Core/EventBus.h"
#include "Core/Events.h"

namespace Merlin {
Application::Application(const std::string &title, int width, int height) {
   SPDLOG_INFO("initializing application");

   window = std::make_unique<Window>(title, width, height);
   guiModule = std::make_unique<GuiModule>(window->getNative());
   renderer = std::make_unique<Renderer>(width, height);

   // lights
   renderer->scene.lights.push_back(std::make_unique<DirectionalLight>(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f)));

   // callbacks
   EventBus::get().on<WindowResizeEvent>([this](const WindowResizeEvent &e) {
      SPDLOG_INFO("WINDOW_RESIZE_EVENT: {},{}", e.width, e.height);
      renderer->resize(e.width, e.height);
   });
   EventBus::get().on<KeyPressedEvent>([this](const KeyPressedEvent &e) {
      const auto &vp = renderer->scene.viewports[0];
      if (!vp.focused)
         return;
      vp.camera->onKeyPress(e.key);
   });
   EventBus::get().on<KeyReleasedEvent>([this](const KeyReleasedEvent &e) {
      const auto &vp = renderer->scene.viewports[0];
      vp.camera->onKeyRelease(e.key);
   });
   EventBus::get().on<MouseMovedEvent>([this](const MouseMovedEvent &e) {
      const auto &vp = renderer->scene.viewports[0];
      if (!vp.focused)
         return;
      vp.camera->onMouseMove(e.dx, e.dy);
   });
   EventBus::get().on<MouseScrolledEvent>([this](const MouseScrolledEvent &e) {
      const auto &vp = renderer->scene.viewports[0];
      if (!vp.focused)
         return;
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

      guiModule->Draw(renderer->scene.viewports.front().framebuffer->colorAttachments[0]);

      window->swapBuffers();
   }
}

void Application::quit() {
   // cleanup and stuff here
   window->Shutdown();
   glfwTerminate();
}
}  // namespace Merlin