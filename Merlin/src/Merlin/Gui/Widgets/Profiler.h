#pragma once

#include "Merlin/Core.h"
#include "Merlin/Gui/Widget.h"

namespace Merlin {
	class MERLIN_API Profiler : public Widget {
	public:
		void OnImGuiRender() override;
	
	private:
		static constexpr int FRAME_HIST_COUNT = 100;
		float m_History[FRAME_HIST_COUNT] = { 0 };
		int m_Offset = 0;
		float m_FrameTimeSum = 0.0f;
		float m_MaxFrameTime = 0.0f;
	};
}