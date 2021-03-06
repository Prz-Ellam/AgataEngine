#ifndef AGATA_VERTEX_BUFFER_H
#define AGATA_VERTEX_BUFFER_H

#include <stdint.h>

namespace Agata {

	class VertexBuffer {
	public:
		VertexBuffer(const void* data, size_t size);
		VertexBuffer(size_t size);
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept;
		~VertexBuffer();
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept;
		void bind() const;
		void unbind() const;
		void sendData(const void* data, size_t size, int64_t offset = 0ll);
	private:
		uint32_t m_ID;
	};

}

#endif