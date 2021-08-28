#ifndef AGATA_FRAME_BUFFER_H
#define AGATA_FRAME_BUFFER_H

#include <stdint.h>

namespace Agata {

	class FrameBuffer {
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		FrameBuffer(const FrameBuffer& other) = delete;
		~FrameBuffer();

		void bind(uint32_t width, uint32_t height) const;
		uint32_t bindColorTexture(uint32_t slot);
		uint32_t bindDepthTexture(uint32_t slot);

		void unbindTexture() const;
		void unbind() const;

	private:
		uint32_t m_ID;
		uint32_t m_ColorTextureID;
		uint32_t m_DepthTextureID;
		uint32_t m_RenderBuffer;

		void createFrameBuffer();
		void createColorTexture(uint32_t width, uint32_t height);
		void createDepthTexture(uint32_t width, uint32_t height);
		void createRenderBuffer(uint32_t width, uint32_t height);

	};

}

#endif