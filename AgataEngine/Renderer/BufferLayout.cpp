#include "BufferLayout.h"

BufferLayout::BufferLayout(uint32_t index, DataType type)
	: index(index), type(type), size(getDataTypeSize(type)), count(getDataCount(type)) {

};

Vertex3D::Vertex3D(glm::vec3 coords, glm::vec2 uv, glm::vec3 normals, glm::vec3 tangents, glm::vec3 bitangents)
	: coords(coords), uv(uv), normals(normals), tangents(tangents), bitangents(bitangents) {

}

Vertex3D::Vertex3D() {}

size_t BufferLayout::getDataTypeSize(DataType type) {

	switch (type) {
		case DataType::Float:	return sizeof(float);
		case DataType::Float2:	return sizeof(float) * 2;
		case DataType::Float3:	return sizeof(float) * 3;
		case DataType::Float4:	return sizeof(float) * 4;
	}

}

uint32_t BufferLayout::getDataCount(DataType type) {

	switch (type) {
		case DataType::Float:	return 1;
		case DataType::Float2:	return 2;
		case DataType::Float3:	return 3;
		case DataType::Float4:	return 4;
	}

}
