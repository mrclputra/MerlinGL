#pragma once

#include "Widget.h"
#include "mepch.h"

namespace Merlin {
	class Console : public Widget {
	public:
		static void AddLog(const std::string& message) {
			s_Logs.push_back(message);
			s_ScrollToBottom = true;
		}

		void Render() override {
            ImGui::Begin("Console");
            if (ImGui::Button("Clear")) s_Logs.clear();
            ImGui::Separator();

            ImGui::BeginChild("ScrollingRegion");
            ImGuiListClipper clipper;
            clipper.Begin((int)s_Logs.size());

            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                    const std::string& line = s_Logs[i];
                    size_t p = line.find("[Error]");
                    ImVec4 col = ImVec4(1, 0.4f, 0.4f, 1);
                    const char* tag = "Error";

                    if (p == std::string::npos) { p = line.find("[Warn]"); col = ImVec4(1, 0.8f, 0.3f, 1); tag = "Warn"; }
                    if (p == std::string::npos) { p = line.find("[Info]"); col = ImVec4(0.2f, 1, 0.2f, 1); tag = "Info"; }

                    if (p != std::string::npos) {
                        ImGui::TextUnformatted(line.substr(0, p + 1).c_str()); ImGui::SameLine(0, 0);
                        ImGui::TextColored(col, tag);                         ImGui::SameLine(0, 0);
                        ImGui::TextUnformatted(line.substr(p + strlen(tag) + 1).c_str());
                    }
                    else {
                        ImGui::TextUnformatted(line.c_str());
                    }
                }
            }
            if (s_ScrollToBottom) { ImGui::SetScrollHereY(1.0f); s_ScrollToBottom = false; }
            ImGui::EndChild();
            ImGui::End();
		}

	private:
		inline static std::vector<std::string> s_Logs;
		inline static bool s_ScrollToBottom;
	};
}
