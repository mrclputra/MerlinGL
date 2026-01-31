#pragma once

#include "mepch.h"
#include <typeindex>

#include "Merlin/Core/Core.h"

namespace Merlin {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		Log
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
	};

	// todo: implement categorical events

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	class MERLIN_API Event {
		friend class EventBus;
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// Category helpers
		// I think these make the API slightly easier to use
		bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & category; }
		bool IsInput() const { return GetCategoryFlags() & EventCategoryInput; }
		bool IsKeyboard() const { return GetCategoryFlags() & EventCategoryKeyboard; }
		bool IsMouse() const { return GetCategoryFlags() & EventCategoryMouse; }
		bool IsApplication() const { return GetCategoryFlags() & EventCategoryApplication; }

		bool handled = false;
	};


	class EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

}