#include "Core/Application.h"
#include "Core/EventBus.h"
#include "Core/Events.h"
#include "Engine/Loader.h"
#include "Engine/Camera.h"
#include "Engine/lights/DirectionalLight.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Merlin {
Application::Application(const std::string &title, int width, int height) {
   SPDLOG_INFO("initializing application");

   window = std::make_unique<Window>(title, width, height);
   guiModule = std::make_unique<GuiModule>(window->getNative());
   renderer = std::make_unique<Renderer>(width, height);
   renderer->createViewport(400, 300); // make a default viewport

   // load models
   // Loader::load("C:/Users/Marcelino/Desktop/tests/meshes/stanford_dragon_pbr/scene.gltf", renderer->scene.registry);

   // lights
   auto sun = renderer->scene.registry.create();
   auto& dir = renderer->scene.registry.emplace<DirectionalLight>(sun);
   dir.color = glm::vec3(1.0f, 0.97f, 0.93f);
   dir.direction = glm::vec3(0.5f, -1.0f, 0.0f);
   // dir.updateLightSpaceMatrix();

   // event callbacks
   EventBus::get().on<LoadModelEvent>([this](const LoadModelEvent &e) {
      loader.wipe(renderer->scene.registry); // clear; todo: change this to be controlled by the scene graph
      loader.load(e.path);
   });
   EventBus::get().on<CreateViewportEvent>([this](const CreateViewportEvent &e) {
      renderer->createViewport(e.width, e.height);
   });
   EventBus::get().on<DeleteViewportEvent>([this](const DeleteViewportEvent &e) {
      if (e.viewportIndex >= renderer->scene.viewports.size()) {
         SPDLOG_ERROR("viewport index to be deleted is out of range");
         return;
      }

      auto &vp = renderer->scene.viewports[e.viewportIndex];
      renderer->scene.registry.destroy(vp.cameraEntity); // delete camera entity from registry

      // remove actual viewport
      renderer->scene.viewports.erase(renderer->scene.viewports.begin() + static_cast<int>(e.viewportIndex));
   });
   EventBus::get().on<ViewportResizeEvent>([this](const ViewportResizeEvent &e) {
      if (e.viewportIndex >= renderer->scene.viewports.size())
         return;
      renderer->resizeViewport(renderer->scene.viewports[e.viewportIndex], static_cast<uint32_t>(e.width), static_cast<uint32_t>(e.height));
   });
   EventBus::get().on<KeyPressedEvent>([this](const KeyPressedEvent &e) {
      auto *vp = renderer->scene.getFocusedViewport();
      if (!vp) return;
      renderer->scene.registry.get<Camera>(vp->cameraEntity).onKeyPress(e.key);
   });
   EventBus::get().on<KeyReleasedEvent>([this](const KeyReleasedEvent &e) {
      // broadcast to every camera
      for (auto [entity, cam] : renderer->scene.registry.view<Camera>().each())
         cam.onKeyRelease(e.key);
   });
   EventBus::get().on<MouseMovedEvent>([this](const MouseMovedEvent &e) {
      auto *vp = renderer->scene.getFocusedViewport();
      if (!vp) return;
      renderer->scene.registry.get<Camera>(vp->cameraEntity).onMouseMove(e.dx, e.dy);
   });

   EventBus::get().on<MouseScrolledEvent>([this](const MouseScrolledEvent &e) {
      auto *vp = renderer->scene.getFocusedViewport();
      if (!vp) return;
      renderer->scene.registry.get<Camera>(vp->cameraEntity).onScroll(e.delta);
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

      for (auto &vp : renderer->scene.viewports) {
         renderer->scene.registry.get<Camera>(vp.cameraEntity).update(delta);
      }

      auto* focusedVp = renderer->scene.getFocusedViewport();
      glfwSetInputMode(window->getNative(), GLFW_CURSOR, focusedVp ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

      // todo: not sure if this is the right place to put it
      loader.poll(renderer->scene.registry);

      window->pollEvents();
      renderer->render();

      // glViewport(0, 0, window->getWidth(), window->getHeight());
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      guiModule->Draw(renderer->scene);

      window->swapBuffers();
   }
}

void Application::quit() {
   // cleanup and stuff here
   window->Shutdown();
   glfwTerminate();
}
}  // namespace Merlin