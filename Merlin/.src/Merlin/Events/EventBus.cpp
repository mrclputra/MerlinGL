#include "mepch.h"
#include "EventBus.h"

namespace Merlin {

	// SubscriptionHandle implementation

	SubscriptionHandle::SubscriptionHandle(EventBus* bus, int id, std::type_index typeIdx)
		: m_Bus(bus), m_Id(id), m_TypeIndex(typeIdx) {}

	SubscriptionHandle::~SubscriptionHandle() {
		Unsubscribe();
	}

	SubscriptionHandle::SubscriptionHandle(SubscriptionHandle&& other) noexcept
		: m_Bus(other.m_Bus), m_Id(other.m_Id), m_TypeIndex(other.m_TypeIndex) {
		other.m_Bus = nullptr;
		other.m_Id = -1;
	}

	SubscriptionHandle& SubscriptionHandle::operator=(SubscriptionHandle&& other) noexcept {
		if (this != &other) {
			Unsubscribe();
			m_Bus = other.m_Bus;
			m_Id = other.m_Id;
			m_TypeIndex = other.m_TypeIndex;
			other.m_Bus = nullptr;
			other.m_Id = -1;
		}
		return *this;
	}

	void SubscriptionHandle::Unsubscribe() {
		if (IsValid()) {
			m_Bus->Unsubscribe(m_Id, m_TypeIndex);
			m_Bus = nullptr;
			m_Id = -1;
		}
	}

	// EventBus implementation

	EventBus& EventBus::Get() {
		static EventBus instance;
		return instance;
	}

	SubscriptionHandle EventBus::OnCategory(int categoryFlags, std::function<void(Event&)> callback) {
		int id = m_NextId++;
		m_CategoryCallbacks.push_back({ id, categoryFlags, callback });
		return SubscriptionHandle(this, id, typeid(void));
	}

	void EventBus::Emit(Event& e) {
		if (e.handled) return;

		// First, dispatch to type-specific callbacks
		auto it = m_Callbacks.find(typeid(e));
		if (it != m_Callbacks.end()) {
			for (auto& base : it->second) {
				if (e.handled) break;
				base->Execute(e);
			}
		}

		// Then, dispatch to category callbacks
		for (auto& catCb : m_CategoryCallbacks) {
			if (e.handled) break;
			if (e.GetCategoryFlags() & catCb.CategoryFlags) {
				catCb.Callback(e);
			}
		}
	}

	void EventBus::Unsubscribe(int id, std::type_index typeIdx) {
		// Try to remove from type-specific callbacks
		if (typeIdx != typeid(void)) {
			auto it = m_Callbacks.find(typeIdx);
			if (it != m_Callbacks.end()) {
				auto& vec = it->second;
				vec.erase(
					std::remove_if(vec.begin(), vec.end(),
						[id](const std::shared_ptr<CallbackBase>& cb) { return cb->ID == id; }),
					vec.end()
				);
			}
		}

		// Also try to remove from category callbacks
		m_CategoryCallbacks.erase(
			std::remove_if(m_CategoryCallbacks.begin(), m_CategoryCallbacks.end(),
				[id](const CategoryCallback& cb) { return cb.ID == id; }),
			m_CategoryCallbacks.end()
		);
	}

}
