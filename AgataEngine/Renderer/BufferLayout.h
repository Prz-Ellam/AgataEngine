#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include <stdint.h>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class DataType {
	Float,
	Float2,
	Float3,
	Float4,
	Int3
};

class Vertex3D {
public:
	Vertex3D(glm::vec3 coords, glm::vec2 uv, glm::vec3 normals, glm::vec3 tangents, glm::vec3 bitangents);
	Vertex3D();
	glm::vec3 coords;
	glm::vec2 uv;
	glm::vec3 normals;
	glm::vec3 tangents;
	glm::vec3 bitangents;
};

class AnimVertex {
public:
	AnimVertex(glm::vec3 coords, glm::vec2 uv, glm::vec3 normals, glm::vec3 tangents, glm::vec3 bitangents, 
		glm::vec3 joints, glm::vec3 weights);
	AnimVertex() = default;
	glm::vec3 coords;
	glm::vec2 uv;
	glm::vec3 normals;
	glm::vec3 tangents;
	glm::vec3 bitangents;
	glm::vec3 joints;
	glm::vec3 weights;
};

class BufferLayout {
public:
	BufferLayout(uint32_t index, DataType type);
	static size_t getDataTypeSize(DataType type);
	static uint32_t getDataCount(DataType type);
	uint32_t index;
	DataType type;
	uint32_t count;
	size_t size;
};

#endif