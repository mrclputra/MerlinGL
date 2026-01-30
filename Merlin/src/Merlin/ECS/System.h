#pragma once

#include "Merlin/Core.h"
#include "Merlin/Events/Event.h"
#include <string>

namespace Merlin {

	// forward declaration
	class Registry;

	class MERLIN_API System {
	public:
		System(const std::string& name = "System") : m_DebugName(name) {}
		virtual ~System() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

		// called by Registry::AddSystem
		void SetRegistry(Registry* registry) { m_Registry = registry; }

	protected:
		Registry* m_Registry = nullptr;
		std::string m_DebugName;
	};

}
