#ifndef MERLINGL_LOADER_H
#define MERLINGL_LOADER_H

#include "Engine/Scene.h"

namespace Merlin {

// this class should not own anything but rather write to existing datastructures
class Loader {
public:
   static void load(Scene& scene);
};

}  // namespace Merlin

#endif  // MERLINGL_LOADER_H
