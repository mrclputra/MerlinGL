#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Engine/Scene.h"
#include "Engine/components/Shader.h"

namespace Merlin {
// todo: we should refactor this class later down the line
//    - the renderer should not own the scenegraph, it should only collect, sort, and batch draw calls
//    - renderer functions should accept pointers to resources it needs but it should never own them as class instances; except the opengl specific ones
//    - for actual ownership it should be managed by the scene graph as an independent instance (maybe owned by the application)
//    - basically this class should be more of an OpenGL draw call functions interface than anything else
class Renderer {
 public:
   Renderer(int width, int height);

   void initialize();
   void render();
   void resize(int width, int height);

   std::shared_ptr<Shader> shader;  // this is going to be the base shader for meshes
   Scene scene;

 private:
   int width;
   int height;
   uint32_t vao = 0;
   uint32_t vbo = 0;
   uint32_t ebo = 0;
};
}  // namespace Merlin
#endif  // MERLINGL_RENDERER_H