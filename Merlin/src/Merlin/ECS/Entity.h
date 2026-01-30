#pragma once

#include "Merlin/Core.h"
#include <cstdint>

namespace Merlin {

	using EntityID = uint64_t;
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
