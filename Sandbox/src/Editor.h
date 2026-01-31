#pragma once
#include <Merlin.h>

#include "Widgets/Console.h"
#include "Widgets/Profiler.h"

class Editor : public Merlin::Layer {
public:
	Editor();
	~Editor();

	void OnAttach() override;
	void OnRender() override;
	void OnGuiRender() override;

	static glm::vec2 GetViewportSize() { return s_ViewportSize; }

private:
	Merlin::Console m_Console;
	Merlin::Profiler m_Profiler;

	// Framebuffer for viewport
	unsigned int m_FBO = 0;
	unsigned int m_ColorTexture = 0;
	unsigned int m_DepthRBO = 0;
	glm::vec2 m_FramebufferSize = { 0, 0 };

	static glm::vec2 s_ViewportSize;

	void CreateFramebuffer(int width, int height);
	void DeleteFramebuffer();
};
