#pragma once

#include "Merlin/Core.h"
#include "Merlin/Events/Event.h"
#include <string>

namespace Merlin {
	class MERLIN_API System {
	public:
		System(const std::string& name = "System") : m_DebugName(name) {}
		virtual ~System() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}