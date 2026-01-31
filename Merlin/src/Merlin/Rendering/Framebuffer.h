#pragma once

#include "Merlin/Core.h"
#include <cstdint>

namespace Merlin {
	class MERLIN_API Framebuffer {
	public:
		Framebuffer(uint32_t width, uint32_t height);
		~Framebuffer();

		void Bind();
		void Unbind();
		void Resize(uint32_t width, uint32_t height);

		uint32_t GetColorAttachment() const { return m_ColorTexture; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		uint32_t m_FBO = 0;
		uint32_t m_ColorTexture = 0;
		uint32_t m_DepthRBO = 0;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		void Create();
		void Delete();
	};
}
