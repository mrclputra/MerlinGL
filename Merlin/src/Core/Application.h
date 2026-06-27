#ifndef MERLINGL_APPLICATION_H
#define MERLINGL_APPLICATION_H

#include "Core/Window.h"
#include "Engine/Renderer.h"
#include "Engine/Loader.h"
#include "GUI/GuiModule.h"

namespace Merlin {
class Application {
 public:
   Application(const std::string &title, int width, int height);
   ~Application();

   void run();
   void quit();

 private:
   std::unique_ptr<Window> window;
   std::unique_ptr<GuiModule> guiModule;
   std::unique_ptr<Renderer> renderer;

   Loader loader;
};
}  // namespace Merlin
#endif  // MERLINGL_APPLICATION_H