#ifndef AGATA_TEXTURE_2D_H
#define AGATA_TEXTURE_2D_H

#include <string>

namespace Agata {

	enum class DefaultTex {
		NormalMap,
		SpecularMap,
		DiffuseMap,
		DuDvMap
	};

	class Texture2D {
	public:
		Texture2D(const std::string& filePath);
		Texture2D(const std::string& filePath, DefaultTex type);
		Texture2D(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) noexcept;
		~Texture2D();
		uint32_t bind(uint32_t slot) const;
		void unbind() const;
	private:
		uint32_t m_ID;
		int m_Width, m_Height, m_BPP;
	};

}

#endif
