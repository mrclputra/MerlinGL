#include "Editor.h"
#include <glad/glad.h>

glm::vec2 Editor::s_ViewportSize = { 800, 600 };

Editor::Editor() : Merlin::Layer("Editor") {
	auto logCallback = [](const std::string& msg) { Merlin::Console::AddLog(msg); };
	Merlin::Logger::getCoreLogger().SetCallback(logCallback);
	Merlin::Logger::getClientLogger().SetCallback(logCallback);
}

Editor::~Editor() {
	DeleteFramebuffer();
}

void Editor::OnAttach() {
	CreateFramebuffer(800, 600);
}

void Editor::CreateFramebuffer(int width, int height) {
	if (width <= 0 || height <= 0) return;

	DeleteFramebuffer();

	m_FramebufferSize = { width, height };

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Color attachment
	glGenTextures(1, &m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

	// Depth attachment
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		MERLIN_CORE_ERROR("Framebuffer is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::DeleteFramebuffer() {
	if (m_FBO) {
		glDeleteFramebuffers(1, &m_FBO);
		m_FBO = 0;
	}
	if (m_ColorTexture) {
		glDeleteTextures(1, &m_ColorTexture);
		m_ColorTexture = 0;
	}
	if (m_DepthRBO) {
		glDeleteRenderbuffers(1, &m_DepthRBO);
		m_DepthRBO = 0;
	}
}

void Editor::OnRender() {
	// Bind framebuffer and render scene to it
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glViewport(0, 0, (int)m_FramebufferSize.x, (int)m_FramebufferSize.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render all entities with MeshRenderer
	auto& registry = Merlin::Application::Get().GetRegistry();
	auto& window = Merlin::Application::Get().GetWindow();
	float aspectRatio = m_FramebufferSize.x / m_FramebufferSize.y;

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::OnGuiRender() {
	// menu bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				Merlin::Application::Get().Quit();
			}
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
		ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	// Viewport window
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	s_ViewportSize = { viewportSize.x, viewportSize.y };

	// Resize framebuffer if needed
	if (viewportSize.x > 0 && viewportSize.y > 0 &&
		(m_FramebufferSize.x != viewportSize.x || m_FramebufferSize.y != viewportSize.y)) {
		CreateFramebuffer((int)viewportSize.x, (int)viewportSize.y);
	}

	// Display the framebuffer texture
	if (m_ColorTexture) {
		ImGui::Image((ImTextureID)(intptr_t)m_ColorTexture, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	ImGui::PopStyleVar();

	// Debug panel
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
