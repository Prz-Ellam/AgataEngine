#include "VertexArray.h"
#include <GL/glew.h>

namespace Agata {

	VertexArray::VertexArray() : m_ID(0) {

		glGenVertexArrays(1, &m_ID);

	}

	VertexArray::VertexArray(VertexArray&& other) noexcept {

		this->m_ID = other.m_ID;
		other.m_ID = 0;

	}

	VertexArray::~VertexArray() {

		glDeleteVertexArrays(1, &m_ID);

	}

	void VertexArray::bind() const {

		glBindVertexArray(m_ID);

	}

	void VertexArray::unbind() const {

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

	void VertexArray::attachIndexBuffer(IndexBuffer* ib) {

		glBindVertexArray(m_ID);
		ib->bind();
		glBindVertexArray(0);

	}

}