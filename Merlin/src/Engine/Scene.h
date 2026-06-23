#ifndef MERLINGL_SCENE_H
#define MERLINGL_SCENE_H

#include "Camera.h"

namespace Merlin {

class Scene {
public:
   Camera camera;
   // std::vector<std::unique_ptr<SceneObject>> objects; // todo: rendering targets
};

} // namespace Merlin

#endif  // MERLINGL_SCENE_H