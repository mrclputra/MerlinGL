#pragma once

#include "Merlin/Gui/Widget.h"

#include <imgui.h>

namespace Merlin {
	class Profiler : public Widget {
	public:
		void Render() override {
			if (!IsVisible) return;

			float currentDeltaTime = ImGui::GetIO().DeltaTime * 1000.0f;

			if (m_Offset == 0) {
				m_FrameTimeSum = 0.0f;
				for (int i = 0; i < FRAME_HIST_COUNT; i++) m_FrameTimeSum += m_History[i];
			}

			m_FrameTimeSum -= m_History[m_Offset];
			m_History[m_Offset] = currentDeltaTime;
			m_FrameTimeSum += m_History[m_Offset];

			float movingAverage = m_FrameTimeSum / (float)FRAME_HIST_COUNT;
			m_Offset = (m_Offset + 1) % FRAME_HIST_COUNT;

			m_MaxFrameTime = 0.0f;
			for (float t : m_History) if (t > m_MaxFrameTime) m_MaxFrameTime = t;

			ImGui::Text("Current: %.2f ms", currentDeltaTime);
			ImGui::Text("Average: %.2f ms (%.1f FPS)", movingAverage, (movingAverage > 0.0f) ? 1000.0f / movingAverage : 0.0f);
			ImGui::PlotLines("##FrameTimeGraph", m_History, FRAME_HIST_COUNT, m_Offset, nullptr,
				0.0f, m_MaxFrameTime + 2.0f, ImVec2(ImGui::GetContentRegionAvail().x, 80));
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Max: %.1fms", m_MaxFrameTime);
		}
	
	private:
		inline static constexpr int FRAME_HIST_COUNT = 100;
		float m_History[FRAME_HIST_COUNT] = { 0 };
		int m_Offset = 0;
		float m_FrameTimeSum = 0.0f;
		float m_MaxFrameTime = 0.0f;
	};
}