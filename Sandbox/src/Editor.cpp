#include "Editor.h"
#include <glad/glad.h>

glm::vec2 Editor::s_ViewportSize = { 800, 600 };

Editor::Editor() : Merlin::Layer("Editor") {
	auto logCallback = [](const std::string& msg) { Merlin::Console::AddLog(msg); };
	Merlin::Logger::getCoreLogger().SetCallback(logCallback);
	Merlin::Logger::getClientLogger().SetCallback(logCallback);
}

void Editor::OnAttach() {
	m_Framebuffer = std::make_unique<Merlin::Framebuffer>(800, 600);

	auto& registry = Merlin::Application::Get().GetRegistry();
	registry.AddSystem<Merlin::RenderSystem>();

	m_CameraEntity = registry.CreateEntity();
	auto& transform = registry.AddComponent<Merlin::Transform>(m_CameraEntity);
	transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
	transform.rotation = glm::vec3(0.0f, -0.0f, 0.0f);
	registry.AddComponent<Merlin::Camera>(m_CameraEntity);
}

void Editor::OnUpdate(float dt) {
	if (!m_ViewportFocused) return;

	auto& registry = Merlin::Application::Get().GetRegistry();
	auto& transform = registry.GetComponent<Merlin::Transform>(m_CameraEntity);
	auto& camera = registry.GetComponent<Merlin::Camera>(m_CameraEntity);

	float speed = 1.0f * dt;
	glm::vec3 front = camera.GetFront(transform);
	glm::vec3 right = camera.GetRight(transform);

	if (Merlin::Input::IsKeyPressed(GLFW_KEY_W)) transform.position += front * speed;
	if (Merlin::Input::IsKeyPressed(GLFW_KEY_S)) transform.position -= front * speed;
	if (Merlin::Input::IsKeyPressed(GLFW_KEY_A)) transform.position -= right * speed;
	if (Merlin::Input::IsKeyPressed(GLFW_KEY_D)) transform.position += right * speed;
	if (Merlin::Input::IsKeyPressed(GLFW_KEY_E)) transform.position.y += speed;
	if (Merlin::Input::IsKeyPressed(GLFW_KEY_Q)) transform.position.y -= speed;

	glm::vec2 mousePos(Merlin::Input::GetMouseX(), Merlin::Input::GetMouseY());

	if (Merlin::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		glm::vec2 delta = mousePos - m_LastMousePos;
		transform.rotation.y += delta.x * 0.1f;
		transform.rotation.x -= delta.y * 0.1f;
		transform.rotation.x = glm::clamp(transform.rotation.x, -89.0f, 89.0f);
	}
	m_LastMousePos = mousePos;
}

void Editor::OnRender() {
	auto& renderSystem = Merlin::Application::Get().GetRegistry().GetSystem<Merlin::RenderSystem>();
	renderSystem.Render(*m_Framebuffer);
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
		//ImGuiDockNodeFlags_PassthruCentralNode |
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
	m_ViewportFocused = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

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
