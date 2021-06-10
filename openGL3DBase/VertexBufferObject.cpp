#include "VertexBufferObject.h"

#include <iostream>

void VertexBufferObject::createVBO(glm::uint32_t _reserveSizeBytes)
{
    if (m_isBufferCreated)
    {
        std::cerr << "This buffer is already created! You need to delete it before re-creating it!" << std::endl;
        return;
    }

    glGenBuffers(1, &m_bufferID);
    m_rawData.reserve(_reserveSizeBytes > 0 ? _reserveSizeBytes : 1024);

    std::cout << "Created vertex buffer object with ID " << m_bufferID << " and initial reserved size " << m_rawData.capacity() << " bytes" << std::endl;
    m_isBufferCreated = true;
}


void VertexBufferObject::bindVBO(GLenum _bufferType)
{
    if (!m_isBufferCreated)
    {
        std::cerr << "This buffer is not created yet! You cannot bind it before you create it!" << std::endl;
        return;
    }

    m_bufferType = _bufferType;
    glBindBuffer(m_bufferType, m_bufferID);
}

void VertexBufferObject::addRawData(const void* _ptrData, uint32_t _dataSizeBytes, int _repeat)
{
    const auto bytesToAdd = _dataSizeBytes * _repeat;
    const auto requiredCapacity = _bytesAdded + bytesToAdd;
    if (requiredCapacity > m_rawData.capacity())
    {
        auto newCapacity = m_rawData.capacity() * 2;
        while (newCapacity < requiredCapacity) {
            newCapacity *= 2;
        }

        std::vector<unsigned char> newRawData;
        newRawData.reserve(newCapacity);
        memcpy(newRawData.data(), m_rawData.data(), _bytesAdded);
        m_rawData = std::move(newRawData);
    }

    for (int i = 0; i < _repeat; i++)
    {
        memcpy(m_rawData.data() + _bytesAdded, _ptrData, _dataSizeBytes);
        _bytesAdded += _dataSizeBytes;
    }
}

void* VertexBufferObject::getRawDataPointer()
{
    return m_rawData.data();
}

void VertexBufferObject::uploadDataToGPU(GLenum _usageHint)
{
    if (!m_isBufferCreated)
    {
        std::cerr << "This buffer is not created yet! Call createVBO before uploading data to GPU!" << std::endl;
        return;
    }

    glBufferData(m_bufferType, _bytesAdded, m_rawData.data(), _usageHint);
    m_isDataUploaded = true;
    m_uploadedDataSize = _bytesAdded;
    _bytesAdded = 0;
}

void* VertexBufferObject::mapBufferToMemory(GLenum _usageHint)
{
    if (!m_isDataUploaded) {
        return nullptr;
    }

    return glMapBuffer(m_bufferType, _usageHint);
}

void* VertexBufferObject::mapSubBufferToMemory(GLenum _usageHint, size_t _offset, size_t _length)
{
    if (!m_isDataUploaded) {
        return nullptr;
    }

    return glMapBufferRange(m_bufferType, _offset, _length, _usageHint);
}

void VertexBufferObject::unmapBuffer()
{
    glUnmapBuffer(m_bufferType);
}

GLuint VertexBufferObject::getBufferID()
{
    return m_bufferID;
}

size_t VertexBufferObject::getBufferSize()
{
    return m_isDataUploaded ? m_uploadedDataSize : _bytesAdded;
}

void VertexBufferObject::deleteVBO()
{
    if (!m_isBufferCreated) {
        return;
    }

    std::cout << "Deleting vertex buffer object with ID " << m_bufferID << "..." << std::endl;
    glDeleteBuffers(1, &m_bufferID);
    m_isDataUploaded = false;
    m_isBufferCreated = false;
}
