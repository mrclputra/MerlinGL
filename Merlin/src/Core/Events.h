#ifndef MERLINGL_EVENTS_H
#define MERLINGL_EVENTS_H

namespace Merlin {
enum class EventType {
   WindowClose,
   WindowResize,
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
//  we currently assume the opengl viewport == window;
//  when/if docking is added in the future, we will need introduce a ViewportResizeEvent emitted by GuiModule when the GetContentRegionAvail() changes
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
struct LoadModelEvent {
   static constexpr EventType type = EventType::LoadModel;
   std::string path;
};
struct ReloadShadersEvent {
   static constexpr EventType type = EventType::ReloadShaders;
};

}  // namespace Merlin

#endif  // MERLINGL_EVENTS_H