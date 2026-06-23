#ifndef MERLINGL_SCENE_H
#define MERLINGL_SCENE_H
#include "Camera.h"
#include "Framebuffer.h"

namespace Merlin {
struct Viewport {
   // consider moving this to own struct
   std::unique_ptr<Camera> camera;
   std::unique_ptr<Framebuffer> framebuffer;
   bool focused = false;
   float width{0};
   float height{0};
};

class Scene {
 public:
   std::vector<Viewport> viewports;  // for multi screen rendering in gui
   // std::vector<std::unique_ptr<SceneObject>> objects; // todo: rendering targets
};
}  // namespace Merlin
#endif  // MERLINGL_SCENE_H