#pragma once 

#include "Merlin/Core/Core.h"

#include <string>
#include <typeindex>

namespace Merlin {
#define ASSET_TYPE(type) \
	std::type_index GetTypeIndex() const override { return typeid(type); } \
	static std::type_index GetStaticTypeIndex() { return typeid(type); }

	class MERLIN_API Asset {
	public:
		Asset() = default;
		Asset(const std::string& path) : m_Path(path) {}
		virtual ~Asset() = default;

		virtual std::type_index GetTypeIndex() const = 0;

		const std::string& GetPath() const { return m_Path; }

	protected:
		std::string m_Path;
		// todo: add runtime loading in main loop through load bool
	};
}