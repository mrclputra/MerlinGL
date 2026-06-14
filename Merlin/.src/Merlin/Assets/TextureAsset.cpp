#include "TextureAsset.h"
#include "Merlin/Core/Logger.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Merlin {
	TextureAsset::TextureAsset(const std::string& path, TextureType type)
		: Asset(path), m_Type(type) {
		loadFromFile(path);
	}

	TextureAsset::~TextureAsset() {
		if (m_ID != 0) {
			glDeleteTextures(1, &m_ID);
		}
	}

	void TextureAsset::Bind(unsigned int slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void TextureAsset::Unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextureAsset::loadFromFile(const std::string& path) {
		glGenTextures(1, &m_ID);

		int channels;
		stbi_set_flip_vertically_on_load(false);
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
}
