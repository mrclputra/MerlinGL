#ifndef MERLINGL_SCENE_H
#define MERLINGL_SCENE_H

#include "Engine/Camera.h"
#include "Engine/Framebuffer.h"

namespace Merlin {

struct Viewport {
   // consider moving this to own file
   // std::unique_ptr<Camera> camera;
   entt::entity cameraEntity{entt::null};
   std::unique_ptr<Framebuffer> framebuffer; // color + depth
   bool focused = false; // OS cursor and input lock
   bool hovered = false; // click-to-focus
   float width{0};
   float height{0};
};

class Scene {
 public:
   std::vector<Viewport> viewports;  // for multi-camera rendering
   entt::registry registry; // ecs

   Viewport* getFocusedViewport() {
      for (auto &vp : viewports)
         if (vp.focused)
            return &vp;
      return nullptr;
   }
   const Viewport* getFocusedViewport() const {
      for (const auto &vp : viewports)
         if (vp.focused)
            return &vp;
      return nullptr;
   }
};

}  // namespace Merlin
#endif  // MERLINGL_SCENE_H