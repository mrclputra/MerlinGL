#ifndef MERLINGL_SCENE_H
#define MERLINGL_SCENE_H

#include "Engine/Camera.h"
#include "Engine/Framebuffer.h"
#include "Engine/lights/Light.h"

namespace Merlin {

struct Viewport {
   // consider moving this to own file
   std::unique_ptr<Camera> camera;
   std::unique_ptr<Framebuffer> framebuffer; // color + depth
   bool focused = false;
   float width{0};
   float height{0};
};

class Scene {
 public:
   std::vector<Viewport> viewports;  // for multi-camera rendering
   std::vector<std::unique_ptr<Light>> lights;
   // std::vector<std::unique_ptr<SceneObject>> objects; // todo: implement rendering targets

};

}  // namespace Merlin
#endif  // MERLINGL_SCENE_H