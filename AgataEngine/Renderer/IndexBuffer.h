#ifndef AGATA_INDEX_BUFFER_H
#define AGATA_INDEX_BUFFER_H

#include <stdint.h>

namespace Agata {

	class IndexBuffer {
	public:
		IndexBuffer(const void* data, size_t size);
		IndexBuffer(size_t size);
		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept;
		~IndexBuffer();
		void bind() const;
		void unbind() const;
		void sendData(const void* data, size_t size, int64_t offset = 0);
	private:
		uint32_t m_ID;
	};

}

#endif