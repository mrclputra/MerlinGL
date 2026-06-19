#include "Window.h"

namespace Merlin {
Window::Window(const std::string &title, const int width, const int height) {
   // initialize glfw window
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   m_Window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);
   glfwMakeContextCurrent(m_Window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

   // setup size callback
   glfwSetWindowUserPointer(m_Window, this);
   glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, const int lwidth, const int lheight) {
      // lambda to set internal width and height
      auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
      self->m_Width = lwidth;
      self->m_Height = lheight;
   });
}

void Window::Shutdown() const {
   glfwDestroyWindow(m_Window);
}

bool Window::shouldClose() const {
   return glfwWindowShouldClose(m_Window);
}

void Window::pollEvents() const {
   glfwPollEvents();
}

void Window::swapBuffers() const {
   glfwSwapBuffers(m_Window);
}

int Window::getWidth() const {
   return m_Width;
}

int Window::getHeight() const {
   return m_Height;
}

GLFWwindow *Window::getNative() const {
   return m_Window;
}
}  // namespace Merlin