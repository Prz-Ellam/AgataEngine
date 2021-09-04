#ifndef AGATA_VERTEX_ARRAY_H
#define AGATA_VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include <stdint.h>

namespace Agata {

	class VertexArray {
	public:
		VertexArray();
		VertexArray(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) noexcept;
		~VertexArray();

		void bind() const;
		void unbind() const;

		void attachVertexBuffer(VertexBuffer* vb, std::vector<BufferLayout> layouts);
		void attachIndexBuffer(IndexBuffer* ib);
	private:
		uint32_t m_ID;
	};

}

#endif