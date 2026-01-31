#pragma once
#include <Merlin.h>

class Editor : public Merlin::Layer {
public:
	Editor();
	~Editor() = default;

	void OnAttach() override;
	void OnRender() override;
	void OnGuiRender() override;

	static glm::vec2 GetViewportSize() { return s_ViewportSize; }

private:
	Merlin::Console m_Console;
	Merlin::Profiler m_Profiler;
	std::unique_ptr<Merlin::Framebuffer> m_Framebuffer;

	static glm::vec2 s_ViewportSize;
};
