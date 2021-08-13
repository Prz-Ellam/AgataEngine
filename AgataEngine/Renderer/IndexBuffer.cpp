#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const void* data, size_t size) {

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

IndexBuffer::~IndexBuffer() {

	glDeleteBuffers(1, &m_ID);

}

void IndexBuffer::bind() const {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

}

void IndexBuffer::unbind() const {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}