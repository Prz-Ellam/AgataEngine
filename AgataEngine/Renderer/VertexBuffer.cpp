#include "VertexBuffer.h"
#include <GL/glew.h>

namespace Agata {

	VertexBuffer::VertexBuffer(const void* data, size_t size) {

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	VertexBuffer::VertexBuffer(size_t size) {

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	VertexBuffer::~VertexBuffer() {

		glDeleteBuffers(1, &m_ID);

	}

	void VertexBuffer::bind() const {

		glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	}

	void VertexBuffer::unbind() const {

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void VertexBuffer::sendData(const void* data, size_t size, int64_t offset) {

		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

}
