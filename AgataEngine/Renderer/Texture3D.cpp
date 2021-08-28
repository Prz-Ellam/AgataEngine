#include <GL/glew.h>
#include "Texture3D.h"
#include <stb/stb_image.h>
#include "Timer.h"

namespace Agata {

	Texture3D::Texture3D(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom,
		const std::string& front, const std::string& back) {

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		stbi_set_flip_vertically_on_load(false);
		std::string textures[6] = { right, left, top, bottom, front, back };
		for (int i = 0; i < 6; i++) {
			uint8_t* buffer = stbi_load(textures[i].c_str(), &m_Width, &m_Height, &m_BPP, 4);

			if (buffer) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				stbi_image_free(buffer);
			}
			else {
				stbi_image_free(buffer);
			}
		}

	}

	Texture3D::~Texture3D() {

		glDeleteTextures(1, &m_ID);

	}

	uint32_t Texture3D::bind(uint32_t slot) const {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		return slot;

	}

	void Texture3D::unbind() const {

		glBindTexture(GL_TEXTURE_2D, 0);

	}

}