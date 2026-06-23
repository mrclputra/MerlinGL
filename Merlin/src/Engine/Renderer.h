#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Camera.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "components/Shader.h"

namespace Merlin {
class Renderer {
public:
   Renderer(int width, int height);

   void initialize();
   void render();
   void resize(int width, int height);

   std::shared_ptr<Shader> shader; // this is going to be the base shader for meshes
   std::shared_ptr<Framebuffer> framebuffer;
   Scene scene;

private:
   int width;
   int height;
   uint32_t vao = 0;
   uint32_t vbo = 0;
   uint32_t ebo = 0;
};
} // namespace Merlin

#endif  // MERLINGL_RENDERER_H