#ifndef MERLINGL_DIRECTIONALLIGHT_H
#define MERLINGL_DIRECTIONALLIGHT_H

#include "Light.h"

namespace Merlin {

class DirectionalLight : public Light {
public:
   DirectionalLight(glm::vec3 direction, glm::vec3 color) : Light(glm::vec3(0.0f), color) {
      this->direction = direction;
      this->nearPlane = 1.0f;
      this->farPlane = 60.0f;

      updateLightSpaceMatrix(); // init
   }

   glm::vec3 direction{};
   glm::mat4 lightSpaceMatrix{};

   float nearPlane;
   float farPlane;

   void updateLightSpaceMatrix() {
      // projection matrix controls the extents of the light map
      // todo: check implementation; current proj and view matrices only allow for shadow maps to be generated for objects near (0,0,0)
      const float extents = 10.0f;
      glm::mat4 lightProj = glm::ortho(-extents, extents, -extents, -extents, nearPlane, farPlane);
      glm::vec3 lightPos = -direction * 40.0f;
      glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      lightSpaceMatrix = lightProj * lightView;
   }

};
}

#endif  // MERLINGL_DIRECTIONALLIGHT_H
