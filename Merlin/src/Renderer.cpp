#include "Renderer.h"

#include "EventBus.h"
#include "Events.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "components/Material.h"
#include "components/Mesh.h"
#include "components/PointCloud.h"
#include "components/Transform.h"
#include "lights/DirectionalLight.h"

namespace Merlin {
Renderer::Renderer(int width, int height,
   std::string vertexMeshShaderPath, std::string fragmentMeshShaderPath,
   std::string vertexPCDShaderPath, std::string fragmentPCDShaderPath) {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_PROGRAM_POINT_SIZE);

   // shader reload lambda
   EventBus::get().on<ReloadShadersEvent>([this, vertexMeshShaderPath, fragmentMeshShaderPath, vertexPCDShaderPath, fragmentPCDShaderPath](const ReloadShadersEvent &e) {
      // deletes and initializes new shaders
      pcdShader = std::make_shared<Shader>(vertexPCDShaderPath, fragmentPCDShaderPath);
      meshShader = std::make_shared<Shader>(vertexMeshShaderPath, fragmentMeshShaderPath);

      // pcdShader = std::make_shared<Shader>(shaderDir + "/pcd.vert", shaderDir + "/pcd.frag");
      // meshShader = std::make_shared<Shader>(shaderDir + "/mesh.vert", shaderDir + "/mesh.frag");
   });

   EventBus::get().emit(ReloadShadersEvent{});
}

void Renderer::render() {
   // todo: implement dynamic multi-viewport rendering
   //    easiest solution right now is to just loop this entire function body once for every viewport
   // auto &vp = scene.viewports.front();

   for (auto &vp : scene.viewports) {
      // bind framebuffer
      vp.framebuffer->bind();

      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      auto &camera = scene.registry.get<Camera>(vp.cameraEntity);  // scene camera

      // mesh shader ---
      meshShader->bind();
      meshShader->setMat4("view", camera.getViewMatrix());
      meshShader->setMat4("projection", camera.getProjectionMatrix(vp.width, vp.height));
      // upload lights
      int numDirLights = 0;
      for (const auto &[entity, dir] : scene.registry.view<DirectionalLight>().each()) {
         std::string base = "dirLights[" + std::to_string(numDirLights) + "]";
         meshShader->setVec3(base + ".direction", dir.direction);
         meshShader->setVec3(base + ".color", dir.color);
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

      // point cloud shader ---
      pcdShader->bind();
      pcdShader->setMat4("view", camera.getViewMatrix());
      pcdShader->setMat4("projection", camera.getProjectionMatrix(vp.width, vp.height));
      for (const auto [entity, t, pcd] : scene.registry.view<Transform, PointCloud>().each()) {
         pcdShader->setMat4("model", t.getTransformMatrix());
         glBindVertexArray(pcd.vao);
         glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(pcd.vertexCount));
      }
      glBindVertexArray(0);
      pcdShader->unbind();

      vp.framebuffer->unbind();
   }
}

void Renderer::createViewport(uint32_t width, uint32_t height) {
   // viewport entity
   Viewport vp;
   vp.cameraEntity = scene.registry.create();
   // todo: make sure to remove this cameraEntity from registry before any full viewport object deletion,
   // todo:    unless entt:: has some kind of mechanism to do that already?
   auto& cam = scene.registry.emplace<Camera>(vp.cameraEntity);
   cam.transform.position = glm::vec3(0.0f, 0.0f, 2.0f);

   FramebufferSpec fbSpec;
   fbSpec.width = width;
   fbSpec.height = height;
   fbSpec.attachments = {TextureFormat::RGBA8, TextureFormat::Depth24Stencil8};
   vp.framebuffer = std::make_unique<Framebuffer>(fbSpec);

   vp.width = static_cast<int>(width);
   vp.height = static_cast<int>(height);

   scene.viewports.push_back(std::move(vp));
}

void Renderer::resizeViewport(Viewport &vp, uint32_t width, uint32_t height) {
   vp.width = width;
   vp.height = height;
   vp.framebuffer->resize(width, height);
}

// // todo: deprecate this function for multi-viewport support
// void Renderer::resize(int width, int height) {
//    this->width = width;
//    this->height = height;
//
//    auto& vp = scene.viewports.front();
//    vp.framebuffer->resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
// }
}  // namespace Merlin
