#include "Texture2D.h"
#include <GL/glew.h>
#include <stb/stb_image.h>

namespace Agata {

	Texture2D::Texture2D(const std::string& filePath) {

		stbi_set_flip_vertically_on_load(true);
		uint8_t* buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (buffer) {
			stbi_image_free(buffer);
		}

	}

	Texture2D::Texture2D(const std::string& filePath, DefaultTex type) {

		bool defaultNorm = false;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		if (!buffer) {

			buffer = new uint8_t[4];

			if (type == DefaultTex::DiffuseMap) {
				memset(buffer, 255, 4);
			}
			else if (type == DefaultTex::NormalMap) {
				memset(buffer, 128, 2);
				memset(buffer + 2, 255, 2);
			}
			else if (type == DefaultTex::SpecularMap) {
				memset(buffer, 255, 4);
			}
			else if (type == DefaultTex::DuDvMap) {
				memset(buffer, 0, 3);
				memset(buffer + 3, 255, 1);
			}

			m_Width = m_Height = 1;
			defaultNorm = true;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		if (buffer && !defaultNorm)
			stbi_image_free(buffer);
		else if (buffer && defaultNorm)
			delete[] buffer;

	}

	Texture2D::Texture2D(Texture2D&& other) noexcept {

		this->m_ID = other.m_ID;
		this->m_Width = other.m_Width;
		this->m_Height = other.m_Height;
		this->m_BPP = other.m_BPP;

		other.m_ID = 0;

	}

	Texture2D::~Texture2D() {

		glDeleteTextures(1, &m_ID);

	}

	uint32_t Texture2D::bind(uint32_t slot) const {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		return slot;

	}

	void Texture2D::unbind() const {

		glBindTexture(GL_TEXTURE_2D, 0);

	}

}