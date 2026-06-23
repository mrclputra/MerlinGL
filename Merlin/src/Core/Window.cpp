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

   // setup size callback
   glfwSetWindowUserPointer(handle, this);
   glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, const int lwidth, const int lheight) {
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      self->width = lwidth;
      self->height = lheight;
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
} // namespace Merlin