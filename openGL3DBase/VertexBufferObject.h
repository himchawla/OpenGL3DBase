#pragma once
#include <glew.h>
#include <fwd.hpp>
#include <vector>

class VertexBufferObject
{
public:

	void createVBO(glm::uint32_t _reserveSizeBytes = 0);
	void bindVBO(GLenum _bufferType = GL_ARRAY_BUFFER);
	void addRawData(const void* _ptrData, uint32_t _dataSizeBytes, int _repeat = 1);

	template<typename T>
	void addData(const T& _ptrObj, int _repeat = 1)
	{
		addRawData((void*)&_ptrObj, sizeof(T), _repeat);
	}

	template<typename T>
	void addUpload(const T& _ptrObj, int _pos, int _repeat = 1)
	{
		createVBO();
		bindVBO();
		addData(_ptrObj, _repeat);
		uploadDataToGPU(GL_STATIC_DRAW);
		GL_ARRAY_BUFFER;

		glEnableVertexAttribArray(_pos);
		glVertexAttribPointer(_pos, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	}

	void* getRawDataPointer();
	void uploadDataToGPU(GLenum _usageHint);
	void* mapBufferToMemory(GLenum _usageHint);
	void* mapSubBufferToMemory(GLenum _usageHint, uint32_t _offset, uint32_t _lenght);
	void unmapBuffer();

	GLuint getBufferID();
	uint32_t getBufferSize();

	void deleteVBO();

private:
	GLuint m_bufferID = 0;
	int m_bufferType;
	uint32_t m_uploadedDataSize;
	uint32_t _bytesAdded = 0;
	std::vector<unsigned char> m_rawData;

	bool m_isBufferCreated = false;
	bool m_isDataUploaded = false;
	
};

