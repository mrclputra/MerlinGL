#ifndef MERLINGL_TEXTURE_H
#define MERLINGL_TEXTURE_H

#include <stb_image.h>

namespace Merlin {
// api wrapper class for opengl textures
class Texture {
public:
   Texture(const std::string& path) {
      this->path = path;

      stbi_set_flip_vertically_on_load(false);
      data = stbi_load(path.c_str(), &width, &height, &channels, 0);
      if (data)
         SPDLOG_INFO("loaded texture {}", path);
   }
   ~Texture() {
      if (!id)
         glDeleteTextures(1, &id);
   }

   void bind(unsigned int slot) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, id);
   }
   void unbind() {
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   unsigned int id{0};
   int width{0};
   int height{0};
   int channels{0};

   // this should always be called on the main thread.
   void upload() {
      glGenTextures(1, &id);

      if (data) {
         GLenum format = GL_RGBA; // expected
         if (channels == 1) format = GL_RED;
         if (channels == 2) format = GL_RG;
         if (channels == 3) format = GL_RGB;
         if (channels == 4) format = GL_RGBA;

         glBindTexture(GL_TEXTURE_2D, id);
         glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); // data should already be filled at this point
         glGenerateMipmap(GL_TEXTURE_2D);

         // texture parameters
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

         stbi_image_free(data);
      } else {
         SPDLOG_ERROR("failed to load texture '{}': {}", path, stbi_failure_reason());
      }
   }

private:
   std::string path;
   unsigned char* data{nullptr};

};
}

#endif  // MERLINGL_TEXTURE_H
