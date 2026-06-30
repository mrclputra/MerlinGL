#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Engine/Scene.h"
#include "Engine/components/Shader.h"

namespace Merlin {
class Renderer {
 public:
   Renderer(int width, int height);

   void initialize();
   void render();
   void resize(int width, int height);

   std::shared_ptr<Shader> pcdShader; // point clouds
   std::shared_ptr<Shader> meshShader; // meshes
   Scene scene;

 private:
   int width;
   int height;
};
}  // namespace Merlin
#endif  // MERLINGL_RENDERER_H