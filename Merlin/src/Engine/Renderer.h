#ifndef MERLINGL_RENDERER_H
#define MERLINGL_RENDERER_H

#include "Engine/Scene.h"
#include "Engine/components/Shader.h"

#include <cstdint>
#include <memory>

namespace Merlin {
class Renderer {
 public:
   Renderer(int width, int height,
      std::string vertexMeshShaderPath, std::string fragmentMeshShaderPath,
      std::string vertexPCDShaderPath, std::string fragmentPCDShaderPath);

   void render();

   std::shared_ptr<Shader> pcdShader; // point clouds
   std::shared_ptr<Shader> meshShader; // meshes
   Scene scene;

   void createViewport(uint32_t width, uint32_t height);
   void resizeViewport(Viewport &vp, uint32_t width, uint32_t height);

};
}  // namespace Merlin
#endif  // MERLINGL_RENDERER_H