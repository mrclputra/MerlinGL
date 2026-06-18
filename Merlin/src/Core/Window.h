#ifndef MERLINGL_WINDOW_H
#define MERLINGL_WINDOW_H

namespace Merlin {
class Window {
public:
   Window(const std::string& title, int width, int height);
   void Shutdown() const;

   bool shouldClose() const; // no-discard?
   void pollEvents() const;
   void swapBuffers() const;

   int getWidth() const;
   int getHeight() const;

   // returns the raw glfw pointer
   GLFWwindow* getNative() const;

private:
   GLFWwindow* m_Window;
   int m_Width;
   int m_Height;
};
} // Merlin

#endif //MERLINGL_WINDOW_H
