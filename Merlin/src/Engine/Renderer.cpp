#include "Engine/Renderer.h"

#include "Core/EventBus.h"
#include "Engine/Camera.h"
#include "Engine/Framebuffer.h"
#include "Engine/components/Material.h"
#include "Engine/components/Mesh.h"
#include "Engine/components/PointCloud.h"
#include "Engine/components/Transform.h"
#include "Engine/lights/DirectionalLight.h"

namespace Merlin {
Renderer::Renderer(int width, int height) {
   this->width = width;
   this->height = height;

   initialize();

   // shader reload lambda
   EventBus::get().on<ReloadShadersEvent>([this](const ReloadShadersEvent &e) {
      // todo: delete and reinitialize the shaders, it should be thread-safe
      pcdShader = std::make_shared<Shader>(SHADER_DIR "/pcd.vert", SHADER_DIR "/pcd.frag");
      meshShader = std::make_shared<Shader>(SHADER_DIR "/mesh.vert", SHADER_DIR "/mesh.frag");
   });
}

void Renderer::initialize() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_PROGRAM_POINT_SIZE);

   pcdShader = std::make_shared<Shader>(SHADER_DIR "/pcd.vert", SHADER_DIR "/pcd.frag");
   meshShader = std::make_shared<Shader>(SHADER_DIR "/mesh.vert", SHADER_DIR "/mesh.frag");

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

   glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // this is the base shader;
   // we will need to add support for multiple shaders and targets in the future
   meshShader->bind();

   // upload camera uniforms
   meshShader->setMat4("view", vp.camera->getViewMatrix());
   meshShader->setMat4("projection", vp.camera->getProjectionMatrix());

   // upload lights
   int numDirLights = 0;
   for (const auto& [entity, dir] : scene.registry.view<DirectionalLight>().each()) {
      std::string base = "dirLights[" + std::to_string(numDirLights) + "]";
      meshShader->setVec3(base + ".direction", dir.direction);
      meshShader->setVec3(base + ".color",     dir.color);
      numDirLights++;
   }
   meshShader->setInt("numDirLights", numDirLights);

   // render meshes
   for (const auto [entity, t, mesh, mat] : scene.registry.view<Transform, Mesh, Material>().each()) {
      if (mat.albedoMap) {
         mat.albedoMap->bind(0);
         meshShader->setInt("uAlbedoMap", 0);
         meshShader->setInt("hasAlbedoMap", 1);
      } else {
         meshShader->setInt("hasAlbedoMap", 0);
      }
      if (mat.normalMap) {
         mat.normalMap->bind(1);
         meshShader->setInt("uNormalMap", 1);
         meshShader->setInt("hasNormalMap", 1);
      } else {
         meshShader->setInt("hasNormalMap", 0);
      }

      meshShader->setMat4("model", t.getTransformMatrix());
      meshShader->setVec3("material.albedo", mat.albedo);
      meshShader->setFloat("material.roughness", mat.roughness);
      meshShader->setFloat("material.metallic", mat.metallic);
      meshShader->setFloat("material.ao", mat.ao);
      glBindVertexArray(mesh.vao);
      glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, nullptr);
   }

   glBindVertexArray(0);
   meshShader->unbind();

   // point clouds
   pcdShader->bind();
   pcdShader->setMat4("view", vp.camera->getViewMatrix());
   pcdShader->setMat4("projection", vp.camera->getProjectionMatrix());

   for (const auto [entity, t, pcd] : scene.registry.view<Transform, PointCloud>().each()) {
      pcdShader->setMat4("model", t.getTransformMatrix());
      glBindVertexArray(pcd.vao);
      glDrawArrays(GL_POINTS, 0, pcd.vertexCount);
   }
   glBindVertexArray(0);
   pcdShader->unbind();

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
