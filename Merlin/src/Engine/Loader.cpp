#include "Engine/Loader.h"
#include "Engine/components/Transform.h"
#include "Engine/components/Mesh.h"
#include "Engine/components/Material.h"

namespace Merlin {

void Loader::load(Scene& scene) {
   // temp, todo: replace with actual assimp loader
   float vertices[] = {
      // position             // color
      -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  // base BL
       0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  // base BR
       0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  // base TR
      -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  // base TL
       0.0f,  0.5f,  0.0f,   1.0f, 0.0f, 1.0f,  // apex
   };

   unsigned int indices[] = {
      0, 1, 2,  // base
      2, 3, 0,
      0, 1, 4,  // sides
      1, 2, 4,
      2, 3, 4,
      3, 0, 4,
   };

   uint32_t vao, vbo, ebo;
   glGenVertexArrays(1, &vao);
   glGenBuffers(1, &vbo);
   glGenBuffers(1, &ebo);

   glBindVertexArray(vao);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);

   auto e = scene.registry.create();
   scene.registry.emplace<Transform>(e);
   scene.registry.emplace<Mesh>(e, vao, vbo, ebo, 18u);
   scene.registry.emplace<Material>(e);
}

}  // namespace Merlin
