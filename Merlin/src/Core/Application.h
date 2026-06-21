#ifndef MERLINGL_APPLICATION_H
#define MERLINGL_APPLICATION_H

#include "Core/Window.h"
#include "Engine/Renderer.h"
#include "GUI/GuiModule.h"

namespace Merlin {
class Application {
 public:
   Application(const std::string &title, int width, int height);
   ~Application();

   void run();
   void quit();

 private:
   std::unique_ptr<Window> m_Window;  // maybe we should rename to avoid confusion?
   std::unique_ptr<GuiModule> m_GuiModule;
   std::unique_ptr<Renderer> m_Renderer;
};
}  // namespace Merlin

#endif  // MERLINGL_APPLICATION_H
