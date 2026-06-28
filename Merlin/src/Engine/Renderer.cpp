#include "Engine/Renderer.h"
#include "Engine/Camera.h"
#include "Engine/Framebuffer.h"
#include "Engine/lights/DirectionalLight.h"
#include "Engine/components/Transform.h"
#include "Engine/components/Mesh.h"
#include "Engine/components/Material.h"

namespace Merlin {
Renderer::Renderer(int width, int height) {
   this->width = width;
   this->height = height;

   initialize();
}

void Renderer::initialize() {
   glEnable(GL_DEPTH_TEST);

   shader = std::make_shared<Shader>("shaders/model.vert", "shaders/model.frag");

   Viewport vp;
   vp.camera = std::make_unique<Camera>();
   vp.camera->transform.position = glm::vec3(0.0f, 0.0f, 2.0f);

   FramebufferSpec fbSpec;
   fbSpec.width = static_cast<uint32_t>(width);
   fbSpec.height = static_cast<uint32_t>(height);
   fbSpec.attachments = {TextureFormat::RGBA8, TextureFormat::Depth24Stencil8};
   vp.framebuffer = std::make_unique<Framebuffer>(fbSpec);

   // setup scene
   scene.viewports.push_back(std::move(vp));
}

void Renderer::render() {
   // bind framebuffer
   auto &vp = scene.viewports.front();
   vp.framebuffer->bind();

   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // this is the base shader;
   // we will need to add support for multiple shaders and targets in the future
   shader->bind();

   // upload camera uniforms
   shader->setMat4("view", vp.camera->getViewMatrix());
   shader->setMat4("projection", vp.camera->getProjectionMatrix());

   // upload lights
   int numDirLights = 0;
   for (const auto& [entity, dir] : scene.registry.view<DirectionalLight>().each()) {
      std::string base = "dirLights[" + std::to_string(numDirLights) + "]";
      shader->setVec3(base + ".direction", dir.direction);
      shader->setVec3(base + ".color",     dir.color);
      numDirLights++;
   }
   shader->setInt("numDirLights", numDirLights);

   // render meshes
   for (const auto [entity, t, mesh, mat] : scene.registry.view<Transform, Mesh, Material>().each()) {
      if (mat.albedoMap) {
         mat.albedoMap->bind(0);
         shader->setInt("uAlbedoMap", 0);
         shader->setInt("hasAlbedoMap", 1);
      } else {
         shader->setInt("hasAlbedoMap", 0);
      }
      if (mat.normalMap) {
         mat.normalMap->bind(1);
         shader->setInt("uNormalMap", 1);
         shader->setInt("hasNormalMap", 1);
      } else {
         shader->setInt("hasNormalMap", 0);
      }

      shader->setMat4("model", t.getTransformMatrix());
      shader->setVec3("material.albedo", mat.albedo);
      shader->setFloat("material.roughness", mat.roughness);
      shader->setFloat("material.metallic", mat.metallic);
      shader->setFloat("material.ao", mat.ao);
      glBindVertexArray(mesh.vao);
      glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
   }

   glBindVertexArray(0);
   shader->unbind();

   vp.framebuffer->unbind();
}

void Renderer::resize(int width, int height) {
   this->width = width;
   this->height = height;

   auto& vp = scene.viewports.front();
   vp.framebuffer->resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
   vp.camera->setViewport(width, height);
}
}  // namespace Merlin
