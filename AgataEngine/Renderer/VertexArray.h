#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include <stdint.h>

class VertexArray {
public:
	VertexArray();
	VertexArray(const VertexArray& other) = delete;
	~VertexArray();

	void bind();
	void unbind();

	void attachVertexBuffer(VertexBuffer* vb, std::vector<BufferLayout> layouts);
	void attachVertexBuffer(VertexBuffer* vb, BufferLayout layout);
	void attachIndexBuffer(IndexBuffer* ib);
private:
	uint32_t m_ID;
};

#endif