#include "mepch.h"
#include "Registry.h"

namespace Merlin {

	Entity Registry::CreateEntity() {
		EntityID id = m_NextEntityID++;
		m_Entities.push_back(id);
		return Entity(id);
	}

	void Registry::DestroyEntity(EntityID entity) {
		// Remove all components for this entity
		for (auto& [typeIdx, componentMap] : m_Components) {
			componentMap.erase(entity);
		}

		// Remove from entity list
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
		if (it != m_Entities.end()) {
			m_Entities.erase(it);
		}
	}

	bool Registry::IsAlive(EntityID entity) const {
		return std::find(m_Entities.begin(), m_Entities.end(), entity) != m_Entities.end();
	}

	void Registry::Update(float deltaTime) {
		for (auto& system : m_Systems) {
			system->OnUpdate(deltaTime);
		}
	}

}
