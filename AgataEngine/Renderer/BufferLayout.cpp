#include "BufferLayout.h"

namespace Agata {

	BufferLayout::BufferLayout(uint32_t index, DataType type)
		: index(index), type(type), size(getDataTypeSize(type)), count(getDataCount(type)) {

	};

	Vertex3D::Vertex3D(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals,
		const glm::vec3& tangents, const glm::vec3& bitangents)
		: coords(coords), uv(uv), normals(normals), tangents(tangents), bitangents(bitangents) {

	}

	AnimVertex::AnimVertex(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals, const glm::vec3& tangents,
		const glm::vec3& bitangents, const glm::vec4& joints, const glm::vec4& weights) 
		: coords(coords), uv(uv), normals(normals), tangents(tangents), bitangents(bitangents), joints(joints), weights(weights) {

	}

	GrassVertex::GrassVertex(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals, const glm::vec3& tangents,
		const glm::vec3& bitangents, float texIndex, float displacement) 
		: coords(coords), uv(uv), normals(normals), tangents(tangents), bitangents(bitangents), texIndex(texIndex), 
		displacement(displacement) {

	}

	size_t BufferLayout::getDataTypeSize(DataType type) {

		switch (type) {
			case DataType::Float:	return sizeof(float);
			case DataType::Float2:	return sizeof(float) * 2;
			case DataType::Float3:	return sizeof(float) * 3;
			case DataType::Float4:	return sizeof(float) * 4;
			case DataType::Int3:	return sizeof(int) * 3;
		}

	}

	uint32_t BufferLayout::getDataCount(DataType type) {

		switch (type) {
			case DataType::Float:	return 1;
			case DataType::Float2:	return 2;
			case DataType::Float3:	return 3;
			case DataType::Float4:	return 4;
			case DataType::Int3:	return 3;
		}

	}

}