#ifndef ONFER_MESH_H
#define ONFER_MESH_H

#include "BufferLayout.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Mesh {
public:
	Mesh(const std::vector<Vertex3D>& vertex, const std::vector<uint32_t>& indices);
	Mesh(const std::vector<AnimVertex>& vertex, const std::vector<uint32_t>& indices);
	Mesh(const std::vector<glm::vec3>& vertex, const std::vector<uint32_t>& indices);
	//Mesh(const Mesh& other);
	Mesh() = default;
	Mesh(Mesh&& other) noexcept;
	~Mesh();
	void draw(Shader* shader);
	uint32_t getIndexCount() const;
	VertexArray* getVertexArray() const;
private:
	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	IndexBuffer* m_IBO;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;
};

#endif