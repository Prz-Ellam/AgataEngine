#ifndef AGATA_TEXTURE_3D_H
#define AGATA_TEXTURE_3D_H

#include <string>

namespace Agata {

	class Texture3D {
	public:
		Texture3D(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom,
			const std::string& front, const std::string& back);
		Texture3D(const Texture3D& other) = delete;
		Texture3D(Texture3D&& other) noexcept;
		~Texture3D();
		uint32_t bind(uint32_t slot) const;
		void unbind() const;
	private:
		uint32_t m_ID;
		int m_Width, m_Height, m_BPP;
	};

}

#endif