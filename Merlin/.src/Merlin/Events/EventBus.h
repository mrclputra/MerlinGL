#pragma once

#include "mepch.h"
#include <typeindex>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Merlin/Core/Core.h"
#include "Event.h"

namespace Merlin {

	class EventBus;

	class MERLIN_API SubscriptionHandle {
	public:
		SubscriptionHandle() = default;
		SubscriptionHandle(EventBus* bus, int id, std::type_index typeIdx);
		~SubscriptionHandle();

		SubscriptionHandle(const SubscriptionHandle&) = delete;
		SubscriptionHandle& operator=(const SubscriptionHandle&) = delete;

		SubscriptionHandle(SubscriptionHandle&& other) noexcept;
		SubscriptionHandle& operator=(SubscriptionHandle&& other) noexcept;

		void Unsubscribe();
		bool IsValid() const { return m_Id >= 0 && m_Bus != nullptr; }

	private:
		EventBus* m_Bus = nullptr;
		int m_Id = -1;
		std::type_index m_TypeIndex = typeid(void);
	};

	class MERLIN_API EventBus {
	public:
		static EventBus& Get();

		template<typename T>
		using EventCallback = std::function<void(T&)>;

		template<typename T>
		SubscriptionHandle On(EventCallback<T> callback) {
			int id = m_NextId++;
			auto wrapper = std::make_shared<CallbackWrapper<T>>(id, callback);
			m_Callbacks[typeid(T)].push_back(wrapper);
			return SubscriptionHandle(this, id, typeid(T));
		}

		SubscriptionHandle OnCategory(int categoryFlags, std::function<void(Event&)> callback);

		void Emit(Event& e);

		void Unsubscribe(int id, std::type_index typeIdx);

	private:
		EventBus() = default;
		~EventBus() = default;

		EventBus(const EventBus&) = delete;
		EventBus& operator=(const EventBus&) = delete;

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

		struct CategoryCallback {
			int ID;
			int CategoryFlags;
			std::function<void(Event&)> Callback;
		};

		std::unordered_map<std::type_index, std::vector<std::shared_ptr<CallbackBase>>> m_Callbacks;
		std::vector<CategoryCallback> m_CategoryCallbacks;
		int m_NextId = 0;

		friend class SubscriptionHandle;
	};

}
