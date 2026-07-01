#ifndef MERLINGL_FRAMEBUFFER_H
#define MERLINGL_FRAMEBUFFER_H

#include <cstdint>

namespace Merlin {
enum class TextureFormat {
   // allowed color attachment formats
   RGBA8,
   RGB16F,
   RedInt,
   Depth24Stencil8
};

struct FramebufferSpec {
   uint32_t width;
   uint32_t height;
   std::vector<TextureFormat> attachments;
};

class Framebuffer {
 public:
   Framebuffer(const FramebufferSpec &spec);
   ~Framebuffer();

   void bind() const;
   void unbind();
   void resize(uint32_t width, uint32_t height);

   FramebufferSpec spec;
   std::vector<uint32_t> colorAttachments;
   uint32_t fbo = 0;
   uint32_t depthRBO = 0;

 private:
   void Create();
   void Delete();
};
}  // namespace Merlin
#endif  // MERLINGL_FRAMEBUFFER_H