#ifndef AGATA_BUFFER_LAYOUT_H
#define AGATA_BUFFER_LAYOUT_H

#include <stdint.h>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Agata {

	enum class DataType {
		Float,
		Float2,
		Float3,
		Float4,
		Int3
	};

	struct Vertex3D {
		Vertex3D(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals, const glm::vec3& tangents, 
			const glm::vec3& bitangents);
		Vertex3D() = default;
		glm::vec3 coords;
		glm::vec2 uv;
		glm::vec3 normals;
		glm::vec3 tangents;
		glm::vec3 bitangents;
	};

	struct AnimVertex {
		AnimVertex(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals, const glm::vec3& tangents, 
			const glm::vec3& bitangents, const glm::vec4& joints, const glm::vec4& weights);
		AnimVertex() = default;
		glm::vec3 coords;
		glm::vec2 uv;
		glm::vec3 normals;
		glm::vec3 tangents;
		glm::vec3 bitangents;
		glm::vec4 joints;
		glm::vec4 weights;
	};

	struct GrassVertex {
	public:
		GrassVertex(const glm::vec3& coords, const glm::vec2& uv, const glm::vec3& normals, const glm::vec3& tangents,
			const glm::vec3& bitangents, float texIndex, float displacement);
		GrassVertex() = default;
		glm::vec3 coords;
		glm::vec2 uv;
		glm::vec3 normals;
		glm::vec3 tangents;
		glm::vec3 bitangents;
		float texIndex;
		float displacement;
	};

	struct BufferLayout {
		BufferLayout(uint32_t index, DataType type);
		static size_t getDataTypeSize(DataType type);
		static uint32_t getDataCount(DataType type);
		uint32_t index;
		DataType type;
		uint32_t count;
		size_t size;
	};

}

#endif