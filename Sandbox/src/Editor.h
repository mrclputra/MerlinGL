#pragma once
#include <Merlin.h>

class Editor : public Merlin::Layer {
public:
	Editor();
	~Editor() = default;

	void OnAttach() override;
	void OnUpdate(float dt) override;
	void OnRender() override;
	void OnGuiRender() override;

	static glm::vec2 GetViewportSize() { return s_ViewportSize; }

private:
	Merlin::Console m_Console;
	Merlin::Profiler m_Profiler;
	std::unique_ptr<Merlin::Framebuffer> m_Framebuffer;

	Merlin::EntityID m_CameraEntity = 0;
	bool m_ViewportFocused = false;
	glm::vec2 m_LastMousePos = glm::vec2(0.0f);

	static glm::vec2 s_ViewportSize;
};
