#pragma once

#include "Asset.h"
#include "Merlin/Logger.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <string>

namespace Merlin {
	enum class TextureType {
		ALBEDO,
		METALLIC_ROUGHNESS,
		NORMAL,
		OCCLUSION,
		EMISSION
	};

	class TextureAsset : public Asset {
	public:
		ASSET_TYPE(TextureAsset)

		TextureAsset(const std::string& path, TextureType type = TextureType::ALBEDO)
			: Asset(path), m_Type(type) {
			loadFromFile(path);
		}

		~TextureAsset() {
			if (m_ID != 0) {
				glDeleteTextures(1, &m_ID);
			}
		}

		void Bind(unsigned int slot = 0) const {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}

		void Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

		unsigned int GetID() const { return m_ID; }
		TextureType GetType() const { return m_Type; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		unsigned int m_ID = 0;
		TextureType m_Type;
		int m_Width = 0;
		int m_Height = 0;

		void loadFromFile(const std::string& path) {
			glGenTextures(1, &m_ID);

			int channels; 
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &channels, 0);

			if (data) {
				GLenum format = GL_RGBA;
				if (channels == 1) format = GL_RED;
				else if (channels == 2) format = GL_RG;
				else if (channels == 3) format = GL_RGB;
				else if (channels == 4) format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, m_ID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else {
				MERLIN_CORE_ERROR("Failed to load texture: {0}", path);
			}
		}
	};
}