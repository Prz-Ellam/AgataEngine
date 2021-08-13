#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <stdint.h>

class VertexBuffer {
public:
	VertexBuffer(const void* data, size_t size);
	VertexBuffer(const VertexBuffer& other) = delete;
	~VertexBuffer();
	void bind() const;
	void unbind() const;
private:
	uint32_t m_ID;
};

#endif