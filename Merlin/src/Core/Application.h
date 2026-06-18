#ifndef MERLINGL_APPLICATION_H
#define MERLINGL_APPLICATION_H

#include "GUI/GuiModule.h"

// todo: use precompiled headers instead
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Merlin {
class Application {
public:
   Application();
   ~Application();

   void run();
   void quit();

private:
   GLFWwindow *window;
   std::unique_ptr<GuiModule> guiModule;
};
} // namespace Merlin

#endif // MERLINGL_APPLICATION_H
