#ifndef MERLINGL_POINTCLOUD_H
#define MERLINGL_POINTCLOUD_H

#include <cstdint>

namespace Merlin {

struct PointCloud {
   uint32_t vao, vbo;
   uint32_t vertexCount;
};

}  // namespace Merlin

#endif  // MERLINGL_POINTCLOUD_H
