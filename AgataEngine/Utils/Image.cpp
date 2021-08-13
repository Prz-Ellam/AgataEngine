//#include "Image.h"
//#include <stb_image.h>
//
//Image::Image(const std::string& filePath) {
//
//	stbi_set_flip_vertically_on_load(true);
//	m_Buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
//
//}
//
//uint8_t* Image::getPixels() {
//
//	return m_Buffer;
//
//}