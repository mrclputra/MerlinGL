#pragma once

#include "Event.h"

namespace Merlin {
	class MERLIN_API LogEvent : public Event {
	public:
		LogEvent(const std::string& message) : m_Message(message) {}
		
		std::string GetMessage() const { return m_Message; }

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(Log)
	
	private:
		std::string m_Message;
	};
}