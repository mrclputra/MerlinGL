#ifndef MERLINGL_EVENTS_H
#define MERLINGL_EVENTS_H

namespace Merlin {
enum class EventType {
   WindowClose,
   WindowResize,
   CreateViewport,
   DeleteViewport,
   ViewportResize,
   KeyPressed,
   KeyReleased,
   MouseMoved,
   MouseScrolled,
   LoadModel,
   ReloadShaders,
};

struct WindowCloseEvent {
   static constexpr EventType type = EventType::WindowClose;
};
struct WindowResizeEvent {
   static constexpr EventType type = EventType::WindowResize;
   int width;
   int height;
};
struct CreateViewportEvent {
   static constexpr EventType type = EventType::CreateViewport;
   int width;
   int height;
};
struct DeleteViewportEvent {
   static constexpr EventType type = EventType::DeleteViewport;
   size_t viewportIndex;
};
struct ViewportResizeEvent {
   static constexpr EventType type = EventType::ViewportResize;
   size_t viewportIndex;
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
struct LoadModelEvent {
   static constexpr EventType type = EventType::LoadModel;
   std::string path;
};
struct ReloadShadersEvent {
   static constexpr EventType type = EventType::ReloadShaders;
};

}  // namespace Merlin

#endif  // MERLINGL_EVENTS_H