#ifndef MERLINGL_FRAMEBUFFER_H
#define MERLINGL_FRAMEBUFFER_H

namespace Merlin {

enum class TextureFormat {
   // allowed framebuffer color attachment formats
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
   void Bind() const;
   static void Unbind();
   void Resize(uint32_t width, uint32_t height);

 private:
   FramebufferSpec m_Spec;
   std::vector<uint32_t> m_ColorAttachments;
   uint32_t m_FBO = 0;
   uint32_t m_DepthRBO = 0;

   void Create();
   void Delete();
};
}  // namespace Merlin

#endif  // MERLINGL_FRAMEBUFFER_H