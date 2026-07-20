#ifndef MERLINGL_EVENTBUS_H
#define MERLINGL_EVENTBUS_H

#include "Core/Events.h"

#include <functional>
#include <unordered_map>
#include <vector>

namespace Merlin {

class EventBus {
 public:
   static EventBus &get() {  // we want a single instance across the application
      static EventBus instance;
      return instance;
   }

   template <typename T>
   void on(std::function<void(const T &)> fn) {
      listeners[T::type].push_back([fn](const void *data) {
         fn(*static_cast<const T *>(data));
      });
   }

   // todo: implement event unbinding to handle stuff like viewport deletion
   // todo:    this would need to properly index the functions vector to delete the associated function, I believe we can just use an iterator

   template <typename T>
   void emit(const T &event) {
      auto it = listeners.find(T::type);
      if (it != listeners.end())
         for (auto &cb : it->second)
            cb(&event);
   }

 private:
   EventBus() = default;
   std::unordered_map<EventType, std::vector<std::function<void(const void *)>>> listeners;
};

}  // namespace Merlin

#endif  // MERLINGL_EVENTBUS_H
