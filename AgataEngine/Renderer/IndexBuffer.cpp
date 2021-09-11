#include "IndexBuffer.h"
#include <GL/glew.h>

namespace Agata {

	IndexBuffer::IndexBuffer(const void* data, size_t size) {

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	IndexBuffer::IndexBuffer(size_t size) {

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {

		this->m_ID = other.m_ID;
		other.m_ID = 0;

	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {

		uint32_t aux = this->m_ID;
		this->m_ID = other.m_ID;
		other.m_ID = aux;
		return *this;

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

	void IndexBuffer::sendData(const void* data, size_t size, int64_t offset) {

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

}