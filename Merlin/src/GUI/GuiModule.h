#ifndef MERLINGL_GUIMODULE_H
#define MERLINGL_GUIMODULE_H

namespace Merlin {
class GuiModule {
public:
   GuiModule(void *native_window); // requires the glfw window to attach to
   ~GuiModule();

   static ImGuiContext *GetContext();

   void Draw(unsigned int windowWidth, unsigned int windowHeight, uint32_t sceneTexture);

   float viewportWidth;
   float viewportHeight;

private:
   std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> ringSink;
};
} // namespace Merlin

#endif  // MERLINGL_GUIMODULE_H