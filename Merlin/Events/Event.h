#pragma once

// todo: we should unify imports across all files
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>

#include <Merlin/Core.h>

namespace Merlin {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// todo: implement categorical events

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	class MERLIN_API Event {
		friend class EventBus;
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool handled = false;
	};


	// This implementation of an EventBus is a modified version from KestrelGL
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

		template<typename T>
		void Publish(const T& event) {
			auto it = m_Callbacks.find(typeid(T));
			if (it == m_Callbacks.end()) return;

			for (auto& base : it->second) {
				if (event.handled) break;
				
				auto wrapper = std::static_pointer_cast<CallbackWrapper<T>>(base);
				wrapper->Callback(event);
			}
		}

	private:
		struct CallbackBase {
			int ID;
			CallbackBase(int id) : ID(id) {}
			virtual ~CallbackBase() = default;
		};

		template<typename T>
		struct CallbackWrapper : public CallbackBase {
			EventCallback<T> Callback;
			CallbackWrapper(int id, EventCallback<T> cb) : CallbackBase(id), Callback(cb) {}
		};

		std::unordered_map<std::type_index, std::vector<std::shared_ptr<CallbackBase>>> m_Callbacks;
		int m_NextId = 0;
	};
}