#ifndef MERLINGL_DIRECTIONALLIGHT_H
#define MERLINGL_DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Merlin {

struct DirectionalLight {
   glm::vec3 direction{0.0f, -1.0f, 0.0f};
   glm::vec3 color{1.0f};
   float nearPlane{1.0f};
   float farPlane{60.0f};
   glm::mat4 lightSpaceMatrix{1.0f};

   void updateLightSpaceMatrix() {
      // projection matrix controls the extents of the light map
      // todo: revisit implementation later; current proj and view matrices only allow for shadow maps to be generated for objects near (0,0,0)
      const float extents = 10.0f;
      glm::mat4 lightProj = glm::ortho(-extents, extents, -extents, extents, nearPlane, farPlane);
      glm::vec3 lightPos = -direction * 40.0f;
      glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      lightSpaceMatrix = lightProj * lightView;
   }
};

}  // namespace Merlin
#endif  // MERLINGL_DIRECTIONALLIGHT_H
