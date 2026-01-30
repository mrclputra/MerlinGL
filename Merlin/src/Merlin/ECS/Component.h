#pragma once

#include "Merlin/Core.h"
#include "Entity.h"
#include <typeindex>

namespace Merlin {

	class MERLIN_API Component {
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual std::type_index GetTypeIndex() const = 0;

		EntityID Owner = NullEntity;
	};

#define COMPONENT_TYPE(type) \
	std::type_index GetTypeIndex() const override { return typeid(type); } \
	static std::type_index GetStaticTypeIndex() { return typeid(type); }

}
