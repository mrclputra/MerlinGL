#include "Editor.h"
#include <glad/glad.h>

glm::vec2 Editor::s_ViewportSize = { 800, 600 };

Editor::Editor() : Merlin::Layer("Editor") {
	auto logCallback = [](const std::string& msg) { Merlin::Console::AddLog(msg); };
	Merlin::Logger::getCoreLogger().SetCallback(logCallback);
	Merlin::Logger::getClientLogger().SetCallback(logCallback);
}

void Editor::OnAttach() {
	// maybe framebuffer management should be an internal thing instead?
	m_Framebuffer = std::make_unique<Merlin::Framebuffer>(800, 600);
}

void Editor::OnRender() {
	m_Framebuffer->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto& registry = Merlin::Application::Get().GetRegistry();
	float aspectRatio = (float)m_Framebuffer->GetWidth() / (float)m_Framebuffer->GetHeight();

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	auto entities = registry.GetEntitiesWithComponent<Merlin::MeshRenderer>();
	for (auto entity : entities) {
		if (!registry.HasComponent<Merlin::Transform>(entity))
			continue;

		auto& transform = registry.GetComponent<Merlin::Transform>(entity);
		auto& renderer = registry.GetComponent<Merlin::MeshRenderer>(entity);

		if (!renderer.mesh || !renderer.shader)
			continue;

		glm::mat4 model = transform.getModelMatrix();

		renderer.shader->Bind();
		renderer.shader->SetMat4("u_Model", model);
		renderer.shader->SetMat4("u_View", view);
		renderer.shader->SetMat4("u_Projection", projection);

		renderer.mesh->Draw();

		renderer.shader->Unbind();
	}

	m_Framebuffer->Unbind();
}

void Editor::OnGuiRender() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Merlin::Application::Get().Quit();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

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
		ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	s_ViewportSize = { viewportSize.x, viewportSize.y };

	if (viewportSize.x > 0 && viewportSize.y > 0) {
		m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		ImGui::Image((ImTextureID)(intptr_t)m_Framebuffer->GetColorAttachment(), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::Begin("Debug");
	ImGui::Text("Hello Sandbox!!!");
	ImGui::Separator();
	m_Profiler.Render();
	ImGui::Separator();

	bool vsync = Merlin::Application::Get().GetWindow().IsVSync();
	if (ImGui::Checkbox("VSync Enabled", &vsync)) {
		Merlin::Application::Get().GetWindow().SetVSync(vsync);
	}

	ImGui::Text("Viewport: %.0f x %.0f", s_ViewportSize.x, s_ViewportSize.y);

	ImGui::End();

	m_Console.Render();
}
