#ifndef MERLINGL_APPLICATION_H
#define MERLINGL_APPLICATION_H

#include "Window.h"
#include "Renderer.h"
#include "Loader.h"
#include "GUI/GuiModule.h"

#include <string>
#include <memory>

namespace Merlin {
class Application {
 public:
   explicit Application(Window &window);
   ~Application();

   void run();

 private:
   Window& window;
   std::unique_ptr<GuiModule> guiModule;
   std::unique_ptr<Renderer> renderer;

   Loader loader;
};
}  // namespace Merlin
#endif  // MERLINGL_APPLICATION_H