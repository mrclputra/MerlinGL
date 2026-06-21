#include "Framebuffer.h"

namespace Merlin {

Framebuffer::Framebuffer(const FramebufferSpec &spec) {
   this->spec = spec;
   Create();
}

Framebuffer::~Framebuffer() {
   Delete();
}

void Framebuffer::Bind() const {
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   glViewport(0, 0, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height));
   // todo: consider storing direct type instead of casting every call
}

void Framebuffer::Unbind() {
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(const uint32_t width, const uint32_t height) {
   if (width == 0 || height == 0)
      return;  // cannot be zero
   if (width == spec.width && height == spec.height)
      return;

   spec.width = width;
   spec.height = height;
   Delete();
   Create();  // reinitialize
}

void Framebuffer::Create() {
   if (spec.width == 0 || spec.height == 0)
      return;

   glGenFramebuffers(1, &fbo);              // allocate framebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, fbo);  // bind

   int colorIndex = 0;

   // iterate through color attachments and check type; bind accordingly
   for (auto attachment : spec.attachments) {
      if (attachment == TextureFormat::RGBA8) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         colorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::RGB16F) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         // i don't know if floats are correct
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height), 0, GL_RGB, GL_FLOAT, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         colorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::RedInt) {
         uint32_t tex;
         glGenTextures(1, &tex);
         glBindTexture(GL_TEXTURE_2D, tex);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height), 0, GL_RED_INTEGER, GL_INT, nullptr);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, tex, 0);
         colorAttachments.push_back(tex);
         colorIndex++;
      } else if (attachment == TextureFormat::Depth24Stencil8) {
         glGenRenderbuffers(1, &depthRBO);
         glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(spec.width), static_cast<GLsizei>(spec.height));
         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
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

   // completeness check
   GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
   if (status != GL_FRAMEBUFFER_COMPLETE)
      SPDLOG_ERROR("framebuffer incomplete: 0x{:x}", status);

   // unbind
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Delete() {
   if (fbo) {
      glDeleteFramebuffers(1, &fbo);
      fbo = 0;
   }
   if (depthRBO) {
      glDeleteRenderbuffers(1, &depthRBO);
      depthRBO = 0;
   }
   if (!colorAttachments.empty()) {
      glDeleteTextures(static_cast<GLsizei>(colorAttachments.size()), colorAttachments.data());
      colorAttachments.clear();
   }
}

}  // namespace Merlin