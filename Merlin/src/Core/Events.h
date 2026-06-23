#ifndef MERLINGL_EVENTS_H
#define MERLINGL_EVENTS_H

namespace Merlin {
enum class EventType {
   WindowClose,
   WindowResize,
   KeyPressed,
   KeyReleased,
   MouseMoved,
   MouseScrolled
};

struct WindowCloseEvent {
   static constexpr EventType type = EventType::WindowClose;
};
struct WindowResizeEvent {
   static constexpr EventType type = EventType::WindowResize;
   int width;
   int height;
};
struct KeyPressedEvent {
   static constexpr EventType type = EventType::KeyPressed;
   int key;
};
struct KeyReleasedEvent {
   static constexpr EventType type = EventType::KeyReleased;
   int key;
};
struct MouseMovedEvent {
   static constexpr EventType type = EventType::MouseMoved;
   float dx;
   float dy;
};
struct MouseScrolledEvent {
   static constexpr EventType type = EventType::MouseScrolled;
   float delta;
};

}  // namespace Merlin

#endif  // MERLINGL_EVENTS_H