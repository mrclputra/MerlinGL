#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Shader.h"

namespace Merlin {
class Renderer {
 public:
   Renderer(int width, int height);

   void initialize();
   void render(float delta);
   void resize(int width, int height);

 private:
   std::shared_ptr<Shader> m_Shader;  // this is going to be the base shader for meshes

   int m_Width;
   int m_Height;
   uint32_t m_VAO = 0;
   uint32_t m_VBO = 0;
};
}  // namespace Merlin

#endif  // MERLINGL_RENDERER_H