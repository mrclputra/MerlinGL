#ifndef MERLINGL_CAMERA_H
#define MERLINGL_CAMERA_H

#include "components/Transform.h"

#include <glm/glm.hpp>
#include <set>

namespace Merlin {
class Camera {
 public:
   Transform transform;
   glm::vec3 front{}, up{}, right{};
   float yaw = -90.0f;
   float pitch = 0.0f;
   float sensitivity = 0.07f;
   float speed = 3.0f; // movement
   float fov = 50.0f;
   float nearPlane = 0.1f;
   float farPlane = 10000.0f;

   explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

   void update(float delta);

   glm::mat4 getViewMatrix() const;
   glm::mat4 getProjectionMatrix(int width, int height) const;

   void rotate(float xOffset, float yOffset);
   void moveForward(float delta);
   void moveBackward(float delta);
   void moveRight(float delta);
   void moveLeft(float delta);
   void moveUp(float delta);
   void moveDown(float delta);

   // todo: attach these to an eventbus later
   void onKeyPress(int key);
   void onKeyRelease(int key);
   void onMouseMove(int dx, int dy);
   void onScroll(int delta);  // controls speed

 private:
   void updateVectors();

   std::set<int> heldKeys;
   glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
};
}  // namespace Merlin
#endif  // MERLINGL_CAMERA_H