#include "Editor.h"

void Editor::OnGuiRender() {
	// menu bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// todo: call event to exit app
			ImGui::MenuItem("Exit", nullptr, nullptr, false);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// build dockspace
	ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
		ImGuiDockNodeFlags_PassthruCentralNode |
		ImGuiDockNodeFlags_NoTabBar
	);

	static bool first_run = true;
	if (first_run) {
		first_run = false;

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);

		ImGui::DockBuilderDockWindow("Console", dock_bottom_id);
		ImGui::DockBuilderDockWindow("Debug", dock_left_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::Begin("Debug");
	ImGui::Text("Hello Sandbox!!!");
	ImGui::Separator();
	m_Profiler.Render();
	ImGui::Separator();

	bool vsync = Merlin::Application::Get().GetWindow().IsVSync();
	if (ImGui::Checkbox("VSync Enabled", &vsync)) {
		Merlin::Application::Get().GetWindow().SetVSync(vsync);
	}

	ImGui::End();

	m_Console.Render();
}