#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <stdint.h>

class IndexBuffer {
public:
	IndexBuffer(const void* data, size_t size);
	IndexBuffer(const IndexBuffer& other) = delete;
	~IndexBuffer();
	void bind() const;
	void unbind() const;
private:
	uint32_t m_ID;
};

#endif