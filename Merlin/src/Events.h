#ifndef MERLINGL_EVENTS_H
#define MERLINGL_EVENTS_H

#include <string>

namespace Merlin {

struct WindowCloseEvent {
};
struct WindowResizeEvent {
   int width;
   int height;
};
struct CreateViewportEvent {
   int width;
   int height;
};
struct DeleteViewportEvent {
   size_t viewportIndex;
};
struct ViewportResizeEvent {
   size_t viewportIndex;
   int width;
   int height;
};
struct KeyPressedEvent {
   int key;
};
struct KeyReleasedEvent {
   int key;
};
struct MouseMovedEvent {
   float dx;
   float dy;
};
struct MouseScrolledEvent {
   float delta;
};
struct LoadModelEvent {
   std::string path;
};
struct ReloadShadersEvent {
};

}  // namespace Merlin

#endif  // MERLINGL_EVENTS_H