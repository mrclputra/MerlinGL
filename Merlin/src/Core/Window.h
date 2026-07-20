#ifndef MERLINGL_WINDOW_H
#define MERLINGL_WINDOW_H

#include <GLFW/glfw3.h>
#include <functional>
#include <string>

namespace Merlin {
class Window {
 public:
   Window(const std::string &title, int width, int height);
   void Shutdown() const;

   bool shouldClose() const;  // no-discard?
   void pollEvents() const;
   void swapBuffers() const;

   int getWidth() const;
   int getHeight() const;

   // returns the raw glfw pointer
   [[nodiscard]] GLFWwindow *getNative() const;

   // TODO: replace this pattern with an actual event bus
   void onKey(std::function<void(int key, bool pressed)> callback);
   void onMouseMove(std::function<void(float dx, float dy)> callback);
   void onScroll(std::function<void(float delta)> callback);

 private:
   GLFWwindow *handle;
   int width;
   int height;

   float lastMouseX = 0.0f;
   float lastMouseY = 0.0f;
   bool firstMouse = true;
};
}  // namespace Merlin

#endif  // MERLINGL_WINDOW_H