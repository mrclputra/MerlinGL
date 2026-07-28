#ifndef MERLINGL_TRANSFORM_H
#define MERLINGL_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Merlin {
class Transform {
 public:
   glm::vec3 position{0.0f};
   glm::vec3 rotation{0.0f};
   glm::vec3 scale{1.0f};

   void translate(const glm::vec3 &delta) {
      position += delta;
   }

   void rotate(const glm::vec3 &delta) {
      rotation += delta;
   }

   void rescale(const glm::vec3 &delta) {
      scale += delta;
   }

   // object space -> world space
   glm::mat4 getTransformMatrix() const {
      auto m = glm::mat4(1.0f);
      m = glm::translate(m, position);
      m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
      m = glm::scale(m, scale);
      return m;
   }
};
}  // namespace Merlin
#endif  // MERLINGL_TRANSFORM_H