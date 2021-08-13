#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>

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
	~Texture2D();
	uint32_t bind(uint32_t slot);
	void unbind();
private:
	uint32_t m_ID;
	int m_Width, m_Height, m_BPP;
};

#endif
