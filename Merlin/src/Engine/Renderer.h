#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Framebuffer.h"
#include "Shader.h"

namespace Merlin {
class Renderer {
 public:
   Renderer(int width, int height);

   void initialize();
   void render();
   void resize(int width, int height);

   std::shared_ptr<Shader> shader;  // this is going to be the base shader for meshes
   std::shared_ptr<Framebuffer> framebuffer;

 private:
   int m_Width;
   int m_Height;
   uint32_t m_VAO = 0;
   uint32_t m_VBO = 0;
};
}  // namespace Merlin

#endif  // MERLINGL_RENDERER_H