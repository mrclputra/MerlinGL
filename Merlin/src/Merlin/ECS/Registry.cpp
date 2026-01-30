#include "mepch.h"
#include "Registry.h"

namespace Merlin {

	Entity Registry::CreateEntity() {
		EntityID id = m_NextEntityID++;
		m_Entities.insert(id);
		return Entity(id);
	}

	void Registry::DestroyEntity(EntityID entity) {
		// remove all components for this entity
		for (auto& [typeIdx, componentMap] : m_Components) {
			componentMap.erase(entity);
		}

		// remove from entity set
		m_Entities.erase(entity);
	}

	bool Registry::IsAlive(EntityID entity) const {
		return m_Entities.find(entity) != m_Entities.end();
	}

	void Registry::Update(float deltaTime) {
		for (auto& system : m_Systems) {
			system->OnUpdate(deltaTime);
		}
	}

}
