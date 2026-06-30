#include "Engine/Camera.h"

#include <algorithm>

namespace Merlin {
Camera::Camera(glm::vec3 position) {
   transform.position = position;
   SPDLOG_INFO("camera created; {}x{}", viewportWidth, viewportHeight);
   updateVectors();
}

void Camera::updateVectors() {
   glm::vec3 v;
   v.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
   v.y = sin(glm::radians(pitch));
   v.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

   // https://www.songho.ca/opengl/gl_camera.html
   front = glm::normalize(v);
   right = glm::normalize(glm::cross(front, worldUp));
   up = glm::normalize(glm::cross(right, front));
}

void Camera::update(float delta) {
   if (heldKeys.contains(GLFW_KEY_W))
      moveForward(delta);
   if (heldKeys.contains(GLFW_KEY_S))
      moveBackward(delta);
   if (heldKeys.contains(GLFW_KEY_A))
      moveLeft(delta);
   if (heldKeys.contains(GLFW_KEY_D))
      moveRight(delta);
   if (heldKeys.contains(GLFW_KEY_E))
      moveUp(delta);
   if (heldKeys.contains(GLFW_KEY_Q))
      moveDown(delta);
   updateVectors();
}

void Camera::onKeyPress(int key) {
   heldKeys.insert(key);
}

void Camera::onKeyRelease(int key) {
   heldKeys.erase(key);
}

void Camera::onMouseMove(int dx, int dy) {
   rotate(static_cast<float>(dx), static_cast<float>(dy));
}

void Camera::onScroll(int delta) {
   constexpr float minSpeed = 2.0f;
   constexpr float maxSpeed = 140.0f;
   constexpr float step = 4.0f;
   speed = std::clamp(speed + (delta > 0 ? step : -step), minSpeed, maxSpeed);
}

glm::mat4 Camera::getViewMatrix() const {
   return glm::lookAt(transform.position, transform.position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
   return glm::perspective(
       glm::radians(fov),
       static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight),
       nearPlane,
       farPlane);
}

void Camera::rotate(float xOffset, float yOffset) {
   yaw += xOffset * sensitivity;
   pitch -= yOffset * sensitivity;  // minus = standard mouse-look (up = positive pitch)

   pitch = glm::clamp(pitch, -89.0f, 89.0f);
   updateVectors();
}

void Camera::moveForward(float delta) {
   transform.position += front * speed * delta;
}

void Camera::moveBackward(float delta) {
   transform.position -= front * speed * delta;
}

void Camera::moveRight(float delta) {
   transform.position += right * speed * delta;
}

void Camera::moveLeft(float delta) {
   transform.position -= right * speed * delta;
}

void Camera::moveUp(float delta) {
   transform.position += up * speed * 0.5f * delta;
}

void Camera::moveDown(float delta) {
   transform.position -= up * speed * 0.5f * delta;
}

void Camera::setViewport(int width, int height) {
   viewportWidth = width;
   viewportHeight = height;
}
}  // namespace Merlin