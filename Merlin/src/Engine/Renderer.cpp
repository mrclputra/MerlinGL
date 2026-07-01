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
   initialize();
   createViewport(static_cast<uint32_t>(width), static_cast<uint32_t>(height));

   // shader reload lambda
   EventBus::get().on<ReloadShadersEvent>([this](const ReloadShadersEvent &e) {
      // deletes and initializes new shaders
      pcdShader = std::make_shared<Shader>(SHADER_DIR "/pcd.vert", SHADER_DIR "/pcd.frag");
      meshShader = std::make_shared<Shader>(SHADER_DIR "/mesh.vert", SHADER_DIR "/mesh.frag");
   });
}

void Renderer::initialize() {
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_PROGRAM_POINT_SIZE);

   pcdShader = std::make_shared<Shader>(SHADER_DIR "/pcd.vert", SHADER_DIR "/pcd.frag");
   meshShader = std::make_shared<Shader>(SHADER_DIR "/mesh.vert", SHADER_DIR "/mesh.frag");
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
         glDrawArrays(GL_POINTS, 0, pcd.vertexCount);
      }
      glBindVertexArray(0);
      pcdShader->unbind();

      vp.framebuffer->unbind();
   }
}

Viewport &Renderer::createViewport(uint32_t width, uint32_t height) {
   // create camera entity, set position, add to registry
   // build framebufferspec and framebuffer; like in initialize, use width and height
   // puchback into scene.viewports, return scene.viewports.back()

   Viewport vp;
   vp.cameraEntity = scene.registry.create();
   auto& cam = scene.registry.emplace<Camera>(vp.cameraEntity);
   // cam.transform.position = glm::vec3(0.0f, 0.0f, 2.0f);

   FramebufferSpec fbSpec;
   fbSpec.width = width;
   fbSpec.height = height;
   fbSpec.attachments = {TextureFormat::RGBA8, TextureFormat::Depth24Stencil8};
   vp.framebuffer = std::make_unique<Framebuffer>(fbSpec);

   vp.width = width;
   vp.height = height;

   scene.viewports.push_back(std::move(vp));
   return scene.viewports.back();
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
