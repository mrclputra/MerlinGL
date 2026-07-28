#ifndef MERLINGL_MESH_H
#define MERLINGL_MESH_H

#include <cstdint>

namespace Merlin {

struct Mesh {
   // note that these are implicitly converted to GLsizei on call
   uint32_t vao, vbo, ebo;
   uint32_t indexCount;
};

}  // namespace Merlin

#endif  // MERLINGL_MESH_H
