#pragma once

#include <Merlin/Core.h>
#include <Merlin/Gui/Widget.h>
#include "mepch.h"

namespace Merlin {
	class MERLIN_API Console : public Widget {
	public:
		static void AddLog(const std::string& message);
		void OnImGuiRender() override;

	private:
		static std::vector<std::string> s_Logs;
		static bool s_ScrollToBottom;
	};
}