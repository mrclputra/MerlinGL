#include "Camera.h"

#include <algorithm>

namespace Merlin {

Camera::Camera(glm::vec3 position) {
   m_Position = position;
   // todo: width and height currently hardcoded, we need to pass them in from window management
   //    use the setViewport function
   SPDLOG_INFO("camera created; {}x{}", m_ViewportWidth, m_ViewportHeight);
   updateVectors();  // init
}

void Camera::updateVectors() {
   glm::vec3 v;
   v.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
   v.y = sin(glm::radians(m_Pitch));
   v.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

   // https://www.songho.ca/opengl/gl_camera.html
   m_Front = glm::normalize(v);
   m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0.0f, 1.0f, 0.0f)));
   m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::update(float delta) {
   if (held_keys.contains(GLFW_KEY_W))
      moveForward(delta);
   if (held_keys.contains(GLFW_KEY_A))
      moveLeft(delta);
   if (held_keys.contains(GLFW_KEY_S))
      moveBackward(delta);
   if (held_keys.contains(GLFW_KEY_D))
      moveRight(delta);
   if (held_keys.contains(GLFW_KEY_E))
      moveUp(delta);
   if (held_keys.contains(GLFW_KEY_Q))
      moveDown(delta);
   updateVectors();
}

void Camera::onKeyPress(int key) {
   held_keys.insert(key);
}

void Camera::onKeyRelease(int key) {
   held_keys.erase(key);
}

void Camera::onMouseMove(int dx, int dy) {
   rotate(static_cast<float>(dx), static_cast<float>(dy));
}

void Camera::onScroll(int delta) {
   constexpr float min_speed = 2.0f;
   constexpr float max_speed = 140.0f;
   constexpr float step = 4.0f;
   m_Speed = std::clamp(m_Speed + (delta > 0 ? step : -step), min_speed, max_speed);
}

glm::mat4 Camera::getViewMatrix() const {
   return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::getProjectionMatrix() const {
   return glm::perspective(glm::radians(m_FOV), static_cast<float>(m_ViewportWidth) / static_cast<float>(m_ViewportHeight), 0.1f,
                           10000.0f  // clip end
   );
}

void Camera::rotate(float xOffset, float yOffset) {
   m_Yaw += xOffset * m_Sensitivity;
   m_Pitch += yOffset * m_Sensitivity;

   m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
   updateVectors();
}

void Camera::moveForward(const float delta) {
   m_Position += m_Front * m_Speed * delta;
}

void Camera::moveBackward(float delta) {
   m_Position -= m_Front * m_Speed * delta;
}

void Camera::moveLeft(float delta) {
   m_Position -= m_Right * m_Speed * delta;
}

void Camera::moveRight(float delta) {
   m_Position += m_Right * m_Speed * delta;
}

void Camera::moveUp(float delta) {
   m_Position -= m_Up * m_Speed * delta;
}

void Camera::moveDown(float delta) {
   m_Position += m_Up * m_Speed * delta;
}

void Camera::setViewport(int width, int height) {
   m_ViewportWidth = width;
   m_ViewportHeight = height;
}
}  // namespace Merlin