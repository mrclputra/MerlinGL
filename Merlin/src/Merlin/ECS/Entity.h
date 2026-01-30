#pragma once

#include "Merlin/Core.h"
#include <cstdint>

namespace Merlin {
	using EntityID = uint32_t;
	constexpr EntityID NullEntity = 0;

	class MERLIN_API Entity {
	public:
		Entity() : m_ID(NullEntity) {}
		explicit Entity(EntityID id) : m_ID(id) {}
		virtual ~Entity() = default;

		EntityID GetID() const { return m_ID; }
		bool IsValid() const { return m_ID != NullEntity; }

		bool operator==(const Entity& other) const { return m_ID == other.m_ID; }
		bool operator!=(const Entity& other) const { return m_ID != other.m_ID; }
		bool operator<(const Entity& other) const { return m_ID < other.m_ID; }

		operator EntityID() const { return m_ID; }

	private:
		EntityID m_ID;
	};
}

// support for unordered containers
namespace std {
	template<>
	struct hash<Merlin::Entity> {
		size_t operator()(const Merlin::Entity& entity) const noexcept {
			return hash<Merlin::EntityID>()(entity.GetID());
		}
	};
}