#pragma once
#include <glew.h>
#include <vector>
#include <glm.hpp>
#include <iostream>

class FBO
{
	void createFBO(glm::uint32_t _reserveSizeBytes = 0);

	void createRenderTexture(glm::uint32_t _reserveSizeBytes = 0)
	{
		if (m_isRenderBufferCreated)
		{
			std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
			return;
		}

		glGenBuffers(1, &m_renderTexture);
		m_rawData.reserve(_reserveSizeBytes > 0 ? _reserveSizeBytes : 1024);

		m_isRenderBufferCreated = true;
	}


	void BindRenderTexture(GLenum _bufferType = GL_TEXTURE_2D)
	{
		if (!m_isRenderBufferCreated)
		{
			std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
			return;
		}

		m_renderBufferType = _bufferType;
		glBindBuffer(m_renderBufferType, m_renderTexture);
	}
	void AllocateImage()
	{
		if (!m_isRenderBufferCreated)
		{
			std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
			return;
		}

		glTexImage2D(m_renderBufferType, 0, GL_RGB, 1000, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap;
	}
	void AttachTexture()
	{
		glFramebufferTexture2D(m_bufferType, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);
	}
	void bindFBO(GLenum _bufferType = GL_FRAMEBUFFER)
	{
		if (!m_isBufferCreated)
		{
			std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
			return;
		}
		m_bufferType = _bufferType;
		glBindBuffer(m_bufferType, m_frameBuffer);
	}
	
	
	void addUpload()
	{
		createRenderTexture();
		BindRenderTexture();
		AllocateImage();
		
		createFBO();
		bindFBO();
		AttachTexture();
		RBO();
		
	}

	void RBO()
	{

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, // must be
			GL_DEPTH24_STENCIL8, //use as depth - stencil buffer
			1000, 1000);
		//viewport width and height;
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, //target
				GL_DEPTH_STENCIL_ATTACHMENT, //attachment
				GL_RENDERBUFFER, //renderbufferTarget
				m_rbo); // render buffer
	}
	
	void deleteFBO()
	{

	}
	

private:
	GLuint m_frameBuffer = 0;
	GLuint m_renderTexture;
	GLuint m_rbo;
	int m_bufferType;
	GLuint m_renderBufferType;
	uint32_t m_uploadedDataSize;
	uint32_t _bytesAdded = 0;
	std::vector<unsigned char> m_rawData;

	bool m_isRenderBufferCreated;
	bool m_isBufferCreated = false;
	bool m_isDataUploaded = false;
};

