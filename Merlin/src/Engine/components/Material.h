#ifndef MERLINGL_MATERIAL_H
#define MERLINGL_MATERIAL_H

namespace Merlin {

// note that materials are uniform per-object
struct Material {
   glm::vec3 albedo{1.0f};
   float roughness{0.5f};
   float metallic{0.0f};
   float ao{1.0f};
};

}  // namespace Merlin

#endif  // MERLINGL_MATERIAL_H
