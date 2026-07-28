#include "Window.h"
#include "EventBus.h"
#include "Events.h"

namespace Merlin {
Window::Window(const std::string &title, const int width, const int height) {
   // initialize glfw window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // windowed maximized

   handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
   glfwMakeContextCurrent(handle);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   glfwGetFramebufferSize(handle, &this->width, &this->height);

   // setup callbacks
   glfwSetWindowUserPointer(handle, this);
   glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, const int lwidth, const int lheight) {
      EventBus::get().emit(WindowResizeEvent{.width = lwidth, .height = lheight});
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      self->width = lwidth;
      self->height = lheight;
   });

   glfwSetKeyCallback(handle, [](GLFWwindow *w, int key, int, int action, int) {
      if (action == GLFW_REPEAT)
         return;
      if (action == GLFW_PRESS)
         EventBus::get().emit(KeyPressedEvent{.key = key});
      else
         EventBus::get().emit(KeyReleasedEvent{.key = key});
   });

   glfwSetCursorPosCallback(handle, [](GLFWwindow *w, double x, double y) {
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(w));
      float fx = (float)x, fy = (float)y;
      if (self->firstMouse) {
         self->lastMouseX = fx;
         self->lastMouseY = fy;
         self->firstMouse = false;
         return;
      }
      float dx = fx - self->lastMouseX, dy = fy - self->lastMouseY;
      self->lastMouseX = fx;
      self->lastMouseY = fy;
      if (dx != 0 || dy != 0)
         EventBus::get().emit(MouseMovedEvent{.dx = dx, .dy = dy});
   });

   // not sure why glfw has xoffset for scroll, but that is just how the API goes
   glfwSetScrollCallback(handle, [](GLFWwindow *, double, double yoffset) {
      EventBus::get().emit(MouseScrolledEvent{.delta = (float)yoffset});
   });
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
}  // namespace Merlin