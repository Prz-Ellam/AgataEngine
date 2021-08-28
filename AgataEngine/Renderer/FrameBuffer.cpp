#include <GL/glew.h>
#include "FrameBuffer.h"

namespace Agata {

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) {

		createFrameBuffer();
		createColorTexture(width, height);
		createDepthTexture(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	FrameBuffer::~FrameBuffer() {

		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(1, &m_ColorTextureID);
		glDeleteTextures(1, &m_DepthTextureID);
		glDeleteRenderbuffers(1, &m_RenderBuffer);

	}

	void FrameBuffer::bind(uint32_t width, uint32_t height) const {

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glViewport(0, 0, width, height);

	}

	void FrameBuffer::unbind() const {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	uint32_t FrameBuffer::bindColorTexture(uint32_t slot) {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
		return slot;

	}

	uint32_t FrameBuffer::bindDepthTexture(uint32_t slot) {

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
		return slot;

	}

	void FrameBuffer::unbindTexture() const {

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void FrameBuffer::createFrameBuffer() {

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

	}

	void FrameBuffer::createColorTexture(uint32_t width, uint32_t height) {

		glGenTextures(1, &m_ColorTextureID);
		glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0);

	}

	void FrameBuffer::createDepthTexture(uint32_t width, uint32_t height) {

		glGenTextures(1, &m_DepthTextureID);
		glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);

	}

	void FrameBuffer::createRenderBuffer(uint32_t width, uint32_t height) {

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

	}

}