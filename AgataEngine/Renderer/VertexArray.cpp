#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray() : m_ID(0) {

	glGenVertexArrays(1, &m_ID);

}

VertexArray::~VertexArray() {

	glDeleteVertexArrays(1, &m_ID);

}

void VertexArray::bind() {

	glBindVertexArray(m_ID);

}

void VertexArray::unbind() {

	glBindVertexArray(0);

}

void VertexArray::attachVertexBuffer(VertexBuffer* vb, std::vector<BufferLayout> layouts) {

	glBindVertexArray(m_ID);
	vb->bind();

	uint32_t offset = 0;
	size_t stride = 0;
	for (int i = 0; i < layouts.size(); i++) {
		stride += layouts[i].size;
	}
	for (const auto& layout : layouts) {
		glEnableVertexAttribArray(layout.index);
		glVertexAttribPointer(layout.index, layout.count, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		offset += layout.size;
	}
	glBindVertexArray(0);

}

void VertexArray::attachVertexBuffer(VertexBuffer* vb, BufferLayout layout) {

	glBindVertexArray(m_ID);
	vb->bind();

	glEnableVertexAttribArray(layout.index);
	glVertexAttribPointer(layout.index, layout.size, (GLenum)layout.type, GL_FALSE, sizeof(Vertex3D), (const void*)0);

	glBindVertexArray(0);

}

void VertexArray::attachIndexBuffer(IndexBuffer* ib) {

	glBindVertexArray(m_ID);
	ib->bind();
	glBindVertexArray(0);

}
