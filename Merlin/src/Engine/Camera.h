#ifndef MERLINGL_CAMERA_H
#define MERLINGL_CAMERA_H

namespace Merlin {
class Camera {
 public:
   explicit Camera(glm::vec3 position);

   void update(float delta);

   glm::mat4 getViewMatrix() const;
   glm::mat4 getProjectionMatrix() const;

   void rotate(float xOffset, float yOffset);  // inputs in screenspace
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

   void setViewport(int width, int height);

 private:
   glm::vec3 m_Position;
   glm::vec3 m_Front, m_Up, m_Right;

   float m_Yaw = -90.0f;
   float m_Pitch = 0.0f;

   float m_Sensitivity = 0.10f;
   float m_Speed = 10.0f;
   float m_FOV = 32.0f;

   void updateVectors();

   std::set<int> held_keys;
   int m_ViewportWidth = 1280;
   int m_ViewportHeight = 720;
};
}  // namespace Merlin

#endif  // MERLINGL_CAMERA_H