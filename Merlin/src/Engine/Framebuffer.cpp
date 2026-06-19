#include "Framebuffer.h"

namespace Merlin {

Framebuffer::Framebuffer(const FramebufferSpec &spec) {
   m_Spec = spec;
   Create();
}

Framebuffer::~Framebuffer() {
   Delete();
}

void Framebuffer::Bind() const {
   glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
   glViewport(0, 0, static_cast<GLsizei>(m_Spec.width), static_cast<GLsizei>(m_Spec.height));
   // todo: consider storing direct type instead of casting every call
}

void Framebuffer::Unbind() {
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(const uint32_t width, const uint32_t height) {
   if (width == 0 || height == 0)
      return;  // cannot be zero
   if (width == m_Spec.width && height == m_Spec.height)
      return;

   m_Spec.width = width;
   m_Spec.height = height;
   Delete();
   Create();  // reinitialize
}

void Framebuffer::Create() {
   if (m_Spec.width == 0 || m_Spec.height == 0)
      return;

   glGenFramebuffers(1, &m_FBO);              // allocate framebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);  // bind

   int colorIndex = 0;

   // iterate through color attachments and check type; bind accordingly
   for (auto attachment : m_Spec.attachments) {
      if (attachment == TextureFormat::RGBA8) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_Spec.width), static_cast<GLsizei>(m_Spec.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         m_ColorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::RGB16F) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         // i don't know if floats are correct
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<GLsizei>(m_Spec.width), static_cast<GLsizei>(m_Spec.height), 0, GL_RGB, GL_FLOAT, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         m_ColorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::RedInt) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<GLsizei>(m_Spec.width), static_cast<GLsizei>(m_Spec.height), 0, GL_RED_INTEGER, GL_INT, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         m_ColorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::Depth24Stencil8) {
         glGenRenderbuffers(1, &m_DepthRBO);
         glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Spec.width), static_cast<GLsizei>(m_Spec.height));
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
      } else {
         SPDLOG_CRITICAL("Unknown framebuffer attachment type");
      }
   }

   // tell opengl which color attachments are active
   std::vector<GLenum> drawBuffers;
   drawBuffers.reserve(colorIndex);
   for (int i = 0; i < colorIndex; i++)
      drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
   glDrawBuffers(static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());

   // glGenTextures(static_cast<GLsizei>(m_ColorAttachments.size()), &m_ColorAttachments[0]);
   // glBindTexture(GL_TEXTURE_2D, m_ColorAttachments[0]);

   // unbind
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Delete() {
   if (m_FBO) {
      glDeleteFramebuffers(1, &m_FBO);
      m_FBO = 0;
   }
   if (m_DepthRBO) {
      glDeleteRenderbuffers(1, &m_DepthRBO);
      m_DepthRBO = 0;
   }
   if (!m_ColorAttachments.empty()) {
      glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
      m_ColorAttachments.clear();
   }
}

}  // namespace Merlin