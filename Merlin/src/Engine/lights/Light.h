#ifndef MERLINGL_LIGHT_H
#define MERLINGL_LIGHT_H

namespace Merlin {

class Light {
 protected:
   Light(const glm::vec3 &position, const glm::vec3 &color) {
      this->color = color;
      transform.position = position;
   }

 public:
   virtual ~Light() = default;  // forces this to be an abstract base class

   Transform transform;
   glm::vec3 color{1.0f};
};

}  // namespace Merlin

#endif  // MERLINGL_LIGHT_H
