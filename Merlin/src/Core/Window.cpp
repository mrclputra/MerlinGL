#include "Window.h"

namespace Merlin {
Window::Window(const std::string &title, const int width, const int height) {
   // initialize glfw window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
   glfwMakeContextCurrent(handle);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   glfwGetFramebufferSize(handle, &this->width, &this->height);

   // setup callbacks
   glfwSetWindowUserPointer(handle, this);
   glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, const int lwidth, const int lheight) {
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      self->width = lwidth;
      self->height = lheight;
   });

   glfwSetKeyCallback(handle, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
      if (action == GLFW_REPEAT)
         return;
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self->onKeyCallback)
         self->onKeyCallback(key, action == GLFW_PRESS);
   });
   glfwSetCursorPosCallback(handle, [](GLFWwindow *window, double x, double y) {
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      const auto fx = static_cast<float>(x);
      const auto fy = static_cast<float>(y);
      if (self->firstMouse) {
         self->lastMouseX = fx;
         self->lastMouseY = fy;
         self->firstMouse = false;
         return;
      }
      float dx = fx - self->lastMouseX;
      float dy = fy - self->lastMouseY;
      self->lastMouseX = fx;
      self->lastMouseY = fy;
      if (self->onMouseMoveCallback && (dx != 0.0f || dy != 0.0f))
         self->onMouseMoveCallback(dx, dy);
   });
   // not sure why glfw has xoffset for scroll, but that is just how the API goes
   glfwSetScrollCallback(handle, [](GLFWwindow *window, double xoffset, double yoffset) {
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      if (self->onScrollCallback)
         self->onScrollCallback(static_cast<float>(yoffset));
   });
}

void Window::onKey(std::function<void(int key, bool pressed)> callback) {
   onKeyCallback = std::move(callback);
}
void Window::onMouseMove(std::function<void(float dx, float dy)> callback) {
   onMouseMoveCallback = std::move(callback);
}
void Window::onScroll(std::function<void(float delta)> callback) {
   onScrollCallback = std::move(callback);
}

void Window::Shutdown() const {
   glfwDestroyWindow(handle);
}

bool Window::shouldClose() const {
   return glfwWindowShouldClose(handle);
}

void Window::pollEvents() const {
   glfwPollEvents();
}

void Window::swapBuffers() const {
   glfwSwapBuffers(handle);
}

int Window::getWidth() const {
   return width;
}

int Window::getHeight() const {
   return height;
}

GLFWwindow *Window::getNative() const {
   return handle;
}
} // namespace Merlin