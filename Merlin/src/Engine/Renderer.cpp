#include "Renderer.h"

namespace Merlin {

Renderer::Renderer(int width, int height) {
   this->width = width;
   this->height = height;

   initialize();
}

void Renderer::initialize() {
   // opengl configs
   glEnable(GL_DEPTH_TEST);

   // load shader from files
   shader = std::make_shared<Shader>("shaders/model.vert", "shaders/model.frag");

   // setup framebuffer
   FramebufferSpec fbSpec;
   fbSpec.width = static_cast<uint32_t>(width);
   fbSpec.height = static_cast<uint32_t>(height);
   fbSpec.attachments = {TextureFormat::RGBA8, TextureFormat::Depth24Stencil8};
   framebuffer = std::make_shared<Framebuffer>(fbSpec);

   // setup scene
   scene.camera.transform.position = glm::vec3(0.0f, 0.0f, 2.0f);

   // debug pyramid,
   // note that these are in NDC coords bcs no transformations are applied in shader
   float vertices[] = {
      // position             // color
      -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    // Base BL
      0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    // Base BR
      0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,    // Base TR
      -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 0.0f,    // Base TL
      0.0f, 0.5f, 0.0f,       1.0f, 0.0f, 1.0f     // Apex
   };

   unsigned int indices[] = {
      0, 1, 2, // base
      2, 3, 0,
      0, 1, 4, // sides
      1, 2, 4,
      2, 3, 4,
      3, 0, 4
   };

   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   // bind vao
   glBindVertexArray(vao);
   // upload vertices
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   // upload indices
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   // upload attributes, stuff to VAO
   // position attribute
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
   glEnableVertexAttribArray(0);
   // color attribute
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

void Renderer::render() {
   // bind framebuffer
   framebuffer->Bind();

   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // update camera
   // scene.camera.update(delta); // todo: wire up delta

   // debug
   shader->bind();

   // upload shader uniforms
   shader->setMat4("view", scene.camera.getViewMatrix());
   shader->setMat4("projection", scene.camera.getProjectionMatrix());

   // render meshes
   glBindVertexArray(vao);
   glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
   glBindVertexArray(0);
   shader->unbind();

   // unbind framebuffer
   framebuffer->Unbind();
}

void Renderer::resize(int width, int height) {
   this->width = width;
   this->height = height;

   // update framebuffer
   framebuffer->Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

   // update camera
   scene.camera.setViewport(width, height);
}

} // namespace Merlin