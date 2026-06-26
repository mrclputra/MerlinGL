#ifndef MERLINGL_MESH_H
#define MERLINGL_MESH_H

namespace Merlin {

struct Mesh {
   uint32_t vao, vbo, ebo;
   uint32_t indexCount;
};

}  // namespace Merlin

#endif  // MERLINGL_MESH_H
