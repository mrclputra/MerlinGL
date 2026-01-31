#include "Framebuffer.h"
#include <glad/glad.h>

namespace Merlin {
	Framebuffer::Framebuffer(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height) {
		Create();
	}

	Framebuffer::~Framebuffer() {
		Delete();
	}

	void Framebuffer::Create() {
		if (m_Width == 0 || m_Height == 0) return;

		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glGenTextures(1, &m_ColorTexture);
		glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

		glGenRenderbuffers(1, &m_DepthRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Delete() {
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

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Width, m_Height);
	}

	void Framebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height) {
		if (width == m_Width && height == m_Height) return;
		if (width == 0 || height == 0) return;

		m_Width = width;
		m_Height = height;
		Delete();
		Create();
	}
}
