#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H

#include <string>

class Texture3D {
public:
	Texture3D(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, 
		const std::string& front, const std::string& back);
	~Texture3D();
	uint32_t bind(uint32_t slot);
	void unbind();
private:
	uint32_t m_ID;
	int m_Width, m_Height, m_BPP;
};

#endif