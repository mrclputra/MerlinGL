#ifndef MERLINGL_MATERIAL_H
#define MERLINGL_MATERIAL_H

#include "Texture.h"

namespace Merlin {

// note that materials are uniform per-object
struct Material {
   glm::vec3 albedo{1.0f};
   float roughness{0.5f};
   float metallic{0.0f};
   float ao{1.0f};

   // textures
   std::shared_ptr<Texture> albedoMap;
   std::shared_ptr<Texture> normalMap;
   std::shared_ptr<Texture> metRoughMap; // gltf standard, not sure if this is right
};

}  // namespace Merlin

#endif  // MERLINGL_MATERIAL_H
