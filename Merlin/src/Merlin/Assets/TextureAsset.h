#pragma once

#include "Asset.h"
#include "Merlin/Core/Core.h"

#include <string>

namespace Merlin {
	// NOTE: TextureAsset shouldn't be header-only cause it uses stb_image functions which are compiled into the dll but not exported
	//	; this approach maintains stb calls stay within the dll boundary
	//	; also the namespace is much cleaner
	
	enum class TextureType {
		ALBEDO,
		METALLIC_ROUGHNESS,
		NORMAL,
		OCCLUSION,
		EMISSION
	};

	class MERLIN_API TextureAsset : public Asset {
	public:
		ASSET_TYPE(TextureAsset)

		TextureAsset(const std::string& path, TextureType type = TextureType::ALBEDO);
		~TextureAsset();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		unsigned int GetID() const { return m_ID; }
		TextureType GetType() const { return m_Type; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		unsigned int m_ID = 0;
		TextureType m_Type;
		int m_Width = 0;
		int m_Height = 0;

		void loadFromFile(const std::string& path);
	};
}
