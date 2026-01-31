#pragma once

#include "Merlin/Core/Core.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <typeindex>

namespace Merlin {

	class MERLIN_API Registry {
	public:
		Registry() = default;
		~Registry() = default;

		void Update(float deltaTime);

		Registry(const Registry&) = delete;
		Registry& operator=(const Registry&) = delete;

		template<typename T>
		std::vector<EntityID> GetEntitiesWithComponent() const;

		// entities management
		Entity CreateEntity();
		void DestroyEntity(EntityID entity);
		bool IsAlive(EntityID entity) const;

		// components management
		template<typename T, typename... Args>
		T& AddComponent(EntityID entity, Args&&... args);

		template<typename T>
		T& GetComponent(EntityID entity);

		template<typename T>
		const T& GetComponent(EntityID entity) const;

		template<typename T>
		bool HasComponent(EntityID entity) const;

		template<typename T>
		void RemoveComponent(EntityID entity);

		// systems management
		template<typename T, typename... Args>
		T& AddSystem(Args&&... args);

		template<typename T>
		T& GetSystem();

		template<typename T>
		void RemoveSystem();

	private:
		EntityID m_NextEntityID = 1;
		std::unordered_set<EntityID> m_Entities;

		// ComponentType -> (EntityID -> Component)
		std::unordered_map<std::type_index,
			std::unordered_map<EntityID, std::unique_ptr<Component>>> m_Components;

		std::vector<std::unique_ptr<System>> m_Systems;
	};

	// template implementations
	// placed them outside the class so that the API is actually readable at a glance

	template<typename T>
	std::vector<EntityID> Registry::GetEntitiesWithComponent() const {
		std::vector<EntityID> result;
		auto typeIt = m_Components.find(typeid(T));
		if (typeIt != m_Components.end()) {
			for (const auto& [entityID, component] : typeIt->second) {
				result.push_back(entityID);
			}
		}
		return result;
	}

	template<typename T, typename... Args>
	T& Registry::AddComponent(EntityID entity, Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->Owner = entity;
		T& ref = *component;

		m_Components[typeid(T)][entity] = std::move(component);
		return ref;
	}

	template<typename T>
	T& Registry::GetComponent(EntityID entity) {
		auto& componentMap = m_Components.at(typeid(T));
		return static_cast<T&>(*componentMap.at(entity));
	}

	template<typename T>
	const T& Registry::GetComponent(EntityID entity) const {
		const auto& componentMap = m_Components.at(typeid(T));
		return static_cast<const T&>(*componentMap.at(entity));
	}

	template<typename T>
	bool Registry::HasComponent(EntityID entity) const {
		auto typeIt = m_Components.find(typeid(T));
		if (typeIt == m_Components.end()) return false;
		return typeIt->second.find(entity) != typeIt->second.end();
	}

	template<typename T>
	void Registry::RemoveComponent(EntityID entity) {
		auto typeIt = m_Components.find(typeid(T));
		if (typeIt != m_Components.end()) {
			typeIt->second.erase(entity);
		}
	}

	template<typename T, typename... Args>
	T& Registry::AddSystem(Args&&... args) {
		static_assert(std::is_base_of<System, T>::value, "T must derive from System");

		auto system = std::make_unique<T>(std::forward<Args>(args)...);
		system->SetRegistry(this);
		T& ref = *system;
		system->OnAttach();
		m_Systems.push_back(std::move(system));
		return ref;
	}

	template<typename T>
	T& Registry::GetSystem() {
		for (auto& system : m_Systems) {
			if (T* casted = dynamic_cast<T*>(system.get())) {
				return *casted;
			}
		}

		return nullptr;
	}

	template<typename T>
	void Registry::RemoveSystem() {
		for (auto it = m_Systems.begin(); it != m_Systems.end(); ++it) {
			if (dynamic_cast<T*>(it->get())) {
				(*it)->OnDetach();
				m_Systems.erase(it);
				return;
			}
		}
	}

}
