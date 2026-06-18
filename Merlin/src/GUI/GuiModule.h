#ifndef MERLINGL_GUIMODULE_H
#define MERLINGL_GUIMODULE_H

#include <imgui.h>

namespace Merlin {
class GuiModule {
public:
   GuiModule(void *native_window); // requires the glfw window to attach to
   ~GuiModule();

   static ImGuiContext *GetContext();

   static void BeginFrame();
   static void EndFrame(unsigned int width, unsigned int height);
};
} // namespace Merlin

#endif // MERLINGL_GUIMODULE_H
