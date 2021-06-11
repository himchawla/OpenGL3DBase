#include "FBO.h"

void FBO::createFBO(glm::uint32_t _reserveSizeBytes)
{
	if (m_isBufferCreated)
	{
		std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
		return;
	}

	glGenBuffers(1, &m_frameBuffer);
	m_rawData.reserve(_reserveSizeBytes > 0 ? _reserveSizeBytes : 1024);

	std::cout << "Created frame buffer object with ID " << m_frameBuffer << " and initial reserved size " << m_rawData.
		capacity() << " bytes" << std::endl;
	m_isBufferCreated = true;
}
