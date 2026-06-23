#ifndef MERLINGL_CAMERA_H
#define MERLINGL_CAMERA_H
#include "components/Transform.h"

namespace Merlin {
class Camera {
public:
   Transform transform{};
   glm::vec3 front{}, up{}, right{};

   float yaw = -90.0f;
   float pitch = 0.0f;

   float sensitivity = 0.07f;
   float speed = 3.0f;
   float fov = 80.0f;
   float near = 0.1f;
   float far = 10000.0f;

   explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

   void update(float delta);

   glm::mat4 getViewMatrix() const;
   glm::mat4 getProjectionMatrix() const;

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
   void onScroll(int delta); // controls speed

   void setViewport(int width, int height);

private:
   void updateVectors();

   std::set<int> heldKeys;
   glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
   int viewportWidth = 1280;
   int viewportHeight = 720;
};
} // namespace Merlin

#endif  // MERLINGL_CAMERA_H