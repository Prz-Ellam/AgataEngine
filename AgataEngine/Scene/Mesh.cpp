#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(const std::vector<Vertex3D>& vertex, const std::vector<uint32_t>& indices) {

	m_VAO = new VertexArray();

	m_VertexCount = vertex.size();
	m_VBO = new VertexBuffer(vertex.data(), m_VertexCount * sizeof(Vertex3D));
	m_IndexCount = indices.size();
	m_IBO = new IndexBuffer(indices.data(), m_IndexCount * sizeof(uint32_t));

	std::vector<BufferLayout> layouts;
	layouts.push_back(BufferLayout(0, DataType::Float3));
	layouts.push_back(BufferLayout(1, DataType::Float2));
	layouts.push_back(BufferLayout(2, DataType::Float3));
	layouts.push_back(BufferLayout(3, DataType::Float3));
	layouts.push_back(BufferLayout(4, DataType::Float3));

	m_VAO->attachVertexBuffer(m_VBO, layouts);
	m_VAO->attachIndexBuffer(m_IBO);

}

Mesh::Mesh(const std::vector<glm::vec3>& vertex, const std::vector<uint32_t>& indices) {

	m_VAO = new VertexArray();

	m_VertexCount = vertex.size();
	m_VBO = new VertexBuffer(vertex.data(), m_VertexCount * sizeof(glm::vec3));
	m_IndexCount = indices.size();
	m_IBO = new IndexBuffer(indices.data(), m_IndexCount * sizeof(uint32_t));

	std::vector<BufferLayout> layouts;
	layouts.push_back(BufferLayout(0, DataType::Float3));

	m_VAO->attachVertexBuffer(m_VBO, layouts);
	m_VAO->attachIndexBuffer(m_IBO);

}

Mesh::Mesh(Mesh&& other) noexcept {

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_IBO = other.m_IBO;
	m_VertexCount = other.m_VertexCount;
	m_IndexCount = other.m_IndexCount;

	other.m_VAO = nullptr;
	other.m_VBO = nullptr;
	other.m_IBO = nullptr;
	other.m_VertexCount = 0;
	other.m_IndexCount = 0;

}

Mesh::~Mesh() {

	delete m_VAO;
	delete m_VBO;
	delete m_IBO;

}

VertexArray* Mesh::getVertexArray() const {

	return m_VAO;

}

uint32_t Mesh::getIndexCount() const {

	return m_IndexCount;

}

void Mesh::draw(Shader* shader) {

	shader->bind();
	m_VAO->bind();

	// glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

}