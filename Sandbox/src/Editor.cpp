#include "Editor.h"
#include <glad/glad.h>

glm::vec2 EditorSystem::s_ViewportSize = { 800, 600 };

EditorSystem::EditorSystem() : Merlin::System("EditorSystem") {
	auto logCallback = [](const std::string& msg) { Merlin::Console::AddLog(msg); };
	Merlin::Logger::getCoreLogger().SetCallback(logCallback);
	Merlin::Logger::getClientLogger().SetCallback(logCallback);
}

void EditorSystem::OnAttach() {
	m_Framebuffer = std::make_unique<Merlin::Framebuffer>(800, 600);

	m_CameraEntity = m_Registry->CreateEntity();
	auto& transform = m_Registry->AddComponent<Merlin::Transform>(m_CameraEntity);
	transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transform.rotation = glm::vec3(0.0f, -0.0f, 0.0f);
	m_Registry->AddComponent<Merlin::Camera>(m_CameraEntity);
}

void EditorSystem::OnUpdate(float dt) {
	if (m_ViewportFocused) {
		auto& transform = m_Registry->GetComponent<Merlin::Transform>(m_CameraEntity);
		auto& camera = m_Registry->GetComponent<Merlin::Camera>(m_CameraEntity);

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

	auto& renderSystem = m_Registry->GetSystem<Merlin::RenderSystem>();
	renderSystem.Render(*m_Framebuffer);

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
		ImGuiDockNodeFlags_NoTabBar
	);

	static bool first_run = true;
	if (first_run) {
		first_run = false;

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.25f, nullptr, &dock_main_id);
		ImGuiID dock_left_bottom_id = ImGui::DockBuilderSplitNode(dock_left_id, ImGuiDir_Down, 0.6f, nullptr, &dock_left_id);

		ImGui::DockBuilderDockWindow("Console", dock_bottom_id);
		ImGui::DockBuilderDockWindow("Stats", dock_left_id);
		ImGui::DockBuilderDockWindow("Inspector", dock_left_bottom_id);
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

	RenderStatsPanel();
	RenderInspectorPanel();

	m_Console.Render();
}

void EditorSystem::RenderStatsPanel() {
	ImGui::Begin("Stats");

	m_Profiler.Render();
	ImGui::Separator();

	size_t entityCount = m_Registry->GetEntityCount();
	size_t transformCount = m_Registry->GetEntitiesWithComponent<Merlin::Transform>().size();
	size_t cameraCount = m_Registry->GetEntitiesWithComponent<Merlin::Camera>().size();
	size_t meshRendererCount = m_Registry->GetEntitiesWithComponent<Merlin::MeshRenderer>().size();

	ImGui::Text("Entities: %zu", entityCount);
	ImGui::Text("  Transform: %zu", transformCount);
	ImGui::Text("  Camera: %zu", cameraCount);
	ImGui::Text("  MeshRenderer: %zu", meshRendererCount);

	ImGui::Separator();

	if (m_Registry->HasComponent<Merlin::Transform>(m_CameraEntity)) {
		auto& camTransform = m_Registry->GetComponent<Merlin::Transform>(m_CameraEntity);
		ImGui::Text("Camera Position");
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "  %.2f, %.2f, %.2f",
			camTransform.position.x, camTransform.position.y, camTransform.position.z);
	}

	ImGui::Separator();

	bool vsync = Merlin::Application::Get().GetWindow().IsVSync();
	if (ImGui::Checkbox("VSync", &vsync)) {
		Merlin::Application::Get().GetWindow().SetVSync(vsync);
	}

	ImGui::Text("Viewport: %.0f x %.0f", s_ViewportSize.x, s_ViewportSize.y);

	ImGui::End();
}

void EditorSystem::RenderInspectorPanel() {
	ImGui::Begin("Inspector");

	if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (Merlin::EntityID entityID : m_Registry->GetAllEntities()) {
				ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
				if (entityID == m_SelectedEntity) {
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
				}

				std::string components;
				if (m_Registry->HasComponent<Merlin::Transform>(entityID)) components += "T";
				if (m_Registry->HasComponent<Merlin::Camera>(entityID)) components += "C";
				if (m_Registry->HasComponent<Merlin::MeshRenderer>(entityID)) components += "M";

				char label[64];
				snprintf(label, sizeof(label), "Entity %llu [%s]", entityID, components.c_str());

				bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entityID, nodeFlags, "%s", label);

				if (ImGui::IsItemClicked()) {
					m_SelectedEntity = entityID;
				}

				if (nodeOpen) {
					ImGuiTreeNodeFlags subFlags = ImGuiTreeNodeFlags_DefaultOpen;

					if (m_Registry->HasComponent<Merlin::Transform>(entityID)) {
						if (ImGui::TreeNodeEx("Transform", subFlags)) {
							auto& transform = m_Registry->GetComponent<Merlin::Transform>(entityID);

							ImGui::Text("Position");
							ImGui::SetNextItemWidth(200.0f);
							ImGui::DragFloat3("##Pos", &transform.position[0], 0.1f);

							ImGui::Text("Rotation");
							ImGui::SetNextItemWidth(200.0f);
							ImGui::DragFloat3("##Rot", &transform.rotation[0], 1.0f);

							ImGui::Text("Scale");
							ImGui::SetNextItemWidth(200.0f);
							ImGui::DragFloat3("##Scale", &transform.scale[0], 0.1f);

							ImGui::TreePop();
						}
					}

					if (m_Registry->HasComponent<Merlin::Camera>(entityID)) {
						if (ImGui::TreeNodeEx("Camera", subFlags)) {
							auto& camera = m_Registry->GetComponent<Merlin::Camera>(entityID);

							ImGui::Text("FOV");
							ImGui::SetNextItemWidth(140.0f);
							ImGui::SliderFloat("##FOV", &camera.fov, 1.0f, 120.0f);

							ImGui::Text("Near Plane");
							ImGui::SetNextItemWidth(140.0f);
							ImGui::DragFloat("##Near", &camera.nearPlane, 0.01f, 0.001f, 10.0f);

							ImGui::Text("Far Plane");
							ImGui::SetNextItemWidth(140.0f);
							ImGui::DragFloat("##Far", &camera.farPlane, 1.0f, 1.0f, 1000.0f);

							ImGui::Checkbox("Active", &camera.isActive);

							ImGui::TreePop();
						}
					}

					if (m_Registry->HasComponent<Merlin::MeshRenderer>(entityID)) {
						if (ImGui::TreeNodeEx("MeshRenderer", subFlags)) {
							auto& meshRenderer = m_Registry->GetComponent<Merlin::MeshRenderer>(entityID);

							if (ImGui::TreeNodeEx("Material", subFlags)) {
								ImGui::Text("Albedo");
								ImGui::ColorEdit4("##Albedo", &meshRenderer.material.albedo[0]);

								ImGui::Text("Metallic");
								ImGui::SetNextItemWidth(140.0f);
								ImGui::SliderFloat("##Metallic", &meshRenderer.material.metallic, 0.0f, 1.0f);

								ImGui::Text("Roughness");
								ImGui::SetNextItemWidth(140.0f);
								ImGui::SliderFloat("##Roughness", &meshRenderer.material.roughness, 0.0f, 1.0f);

								if (meshRenderer.material.hasAlbedoMap()) {
									ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f), "Albedo Map: Yes");
								}
								if (meshRenderer.material.hasNormalMap()) {
									ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f), "Normal Map: Yes");
								}
								if (meshRenderer.material.hasMetallicRoughnessMap()) {
									ImGui::TextColored(ImVec4(0.5f, 0.8f, 0.5f, 1.0f), "MetRough Map: Yes");
								}

								ImGui::TreePop();
							}

							ImGui::TreePop();
						}
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	ImGui::End();
}
