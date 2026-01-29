#pragma once

// Event system entry point
// include this file for full event support

#include "Event.h"
#include "EventBus.h"
#include "AppEvents.h"
#include "InputEvents.h"
#include "LogEvent.h"

namespace Merlin {
namespace Events {
	// Event type aliases for IDE discoverability, i.e. easier for me to use :)
	// Type Merlin::Events:: to see all available event types

	// Window events
	using WindowClose = WindowCloseEvent;
	using WindowResize = WindowResizeEvent;
	// Application lifecycle events
	using AppTick = AppTickEvent;
	using AppUpdate = AppUpdateEvent;
	using AppRender = AppRenderEvent;
	// Keyboard events
	using KeyPressed = KeyPressedEvent;
	using KeyReleased = KeyReleasedEvent;
	// Mouse events
	using MouseMoved = MouseMovedEvent;
	using MouseScrolled = MouseScrolledEvent;
	using MouseButtonPressed = MouseButtonPressedEvent;
	using MouseButtonReleased = MouseButtonReleasedEvent;
	// System events
	using Log = LogEvent;

	// Convenience functions for event subscription
	// Subscribe to a specific event type
	template<typename T>
	inline SubscriptionHandle On(std::function<void(T&)> callback) {
		return EventBus::Get().On<T>(callback);
	}

	inline SubscriptionHandle OnInput(std::function<void(Event&)> callback) {
		return EventBus::Get().OnCategory(EventCategoryInput, callback);
	}
	inline SubscriptionHandle OnKeyboard(std::function<void(Event&)> callback) {
		return EventBus::Get().OnCategory(EventCategoryKeyboard, callback);
	}
	inline SubscriptionHandle OnMouse(std::function<void(Event&)> callback) {
		return EventBus::Get().OnCategory(EventCategoryMouse, callback);
	}
	inline SubscriptionHandle OnApplication(std::function<void(Event&)> callback) {
		return EventBus::Get().OnCategory(EventCategoryApplication, callback);
	}

	// Emit an event to all subscribers
	inline void Emit(Event& event) {
		EventBus::Get().Emit(event);
	}

} // namespace Events
} // namespace Merlin
