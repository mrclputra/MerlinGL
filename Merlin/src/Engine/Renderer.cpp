#include "Renderer.h"

namespace Merlin {

Renderer::Renderer(int width, int height) {
   m_Width = width;
   m_Height = height;

   initialize();
}

void Renderer::initialize() {
   // opengl configs
   glEnable(GL_DEPTH_TEST);

   // load shader from files
   shader = std::make_shared<Shader>("shaders/model.vert", "shaders/model.frag");

   // setup framebuffer
   FramebufferSpec fbSpec;
   fbSpec.width = static_cast<uint32_t>(m_Width);
   fbSpec.height = static_cast<uint32_t>(m_Height);
   fbSpec.attachments = {TextureFormat::RGBA8, TextureFormat::Depth24Stencil8};
   framebuffer = std::make_shared<Framebuffer>(fbSpec);

   // setup camera

   // debug triangle,
   // note that these are in NDC coords bcs no transformations are applied in shader
   float vertices[] = {
       -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
       0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f
   };

   glGenVertexArrays(1, &m_VAO);
   glGenBuffers(1, &m_VBO);

   glBindVertexArray(m_VAO);

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   // color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

void Renderer::render() {
   // bind framebuffer
   framebuffer->Bind();

   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // update camera
   // bind shader
   // upload shader uniforms
   // render meshes
   // unbind shader

   // debug
   shader->bind();
   glBindVertexArray(m_VAO);
   glDrawArrays(GL_TRIANGLES, 0, 3);
   glBindVertexArray(0);
   shader->unbind();

   // unbind framebuffer
   framebuffer->Unbind();
}

void Renderer::resize(int width, int height) {
   m_Width = width;
   m_Height = height;

   // update framebuffer
   framebuffer->Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

   // update camera
}

}  // namespace Merlin