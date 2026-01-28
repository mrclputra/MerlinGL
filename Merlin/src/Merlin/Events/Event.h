#pragma once

#include "mepch.h"
#include <typeindex>

#include "Merlin/Core.h"

namespace Merlin {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
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

		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

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

	// This implementation of an EventBus is a modified version from KestrelGL
	// TODO: I should make this asynchronous or threaded
	//	as the current implementation is blocking

	// in the current implementation, we do not need an EventBus as the layers should exclusively handle event logic
	// but if we want events in the future to handle engine-wide events, we can use this

	class EventBus {
	public:
		template<typename T>
		using EventCallback = std::function<void(T&)>;

		template<typename T>
		int Subscribe(EventCallback<T> callback) {
			int id = m_NextId++;
			auto wrapper = std::make_shared<CallbackWrapper<T>>(id, callback);
			m_Callbacks[typeid(T)].push_back(wrapper);
			return id;
		}

		void Publish(Event& e) {
			auto it = m_Callbacks.find(typeid(e));
			if (it == m_Callbacks.end()) return;

			for (auto& base : it->second) {
				if (e.handled) break;
				base->Execute(e); // Calls the specific wrapper
			}
		}

	private:
		struct CallbackBase {
			int ID;
			CallbackBase(int id) : ID(id) {}
			virtual ~CallbackBase() = default;
			virtual void Execute(Event& e) = 0;
		};

		template<typename T>
		struct CallbackWrapper : public CallbackBase {
			EventCallback<T> Callback;
			CallbackWrapper(int id, EventCallback<T> cb) : CallbackBase(id), Callback(cb) {}

			void Execute(Event& e) override {
				Callback(static_cast<T&>(e));
			}
		};

		std::unordered_map<std::type_index, std::vector<std::shared_ptr<CallbackBase>>> m_Callbacks;
		int m_NextId = 0;
	};
}