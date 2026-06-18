#ifndef MERLINGL_GUIMODULE_H
#define MERLINGL_GUIMODULE_H

namespace Merlin {
class GuiModule {
public:
   GuiModule(void *native_window); // requires the glfw window to attach to
   ~GuiModule();

   static ImGuiContext *GetContext();

   static void Draw(unsigned int windowWidth, unsigned int windowHeight);
};
} // namespace Merlin

#endif // MERLINGL_GUIMODULE_H
