#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.h"

#include <iostream>
#include <mutex>

bool Texture::loadTexture2D(const std::string& fileName, bool generateMipmaps)
{
	stbi_set_flip_vertically_on_load(1);
	const auto imageData = stbi_load(fileName.c_str(), &_width, &_height, &_bytesPerPixel, 0);
	if (imageData == nullptr)
	{
		std::cout << "Failed to load image " << fileName << "!" << std::endl;
		return false;
	}
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	GLenum internalFormat = 0;
	GLenum format = 0;
	if (_bytesPerPixel == 4) {
		internalFormat = format = GL_RGBA;
	}
	else if (_bytesPerPixel == 3) {
		internalFormat = format = GL_RGB;
	}
	else if (_bytesPerPixel == 1) {
		internalFormat = format = GL_ALPHA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, GL_UNSIGNED_BYTE, imageData);

	if (generateMipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(imageData);
	_isLoaded = true;
	return true;
}

void Texture::bind(int textureUnit) const
{
	if (!isLoadedCheck()) {
		return;
	}

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::deleteTexture()
{
	if (!isLoadedCheck()) {
		return;
	}

	glDeleteTextures(1, &_textureID);
	_isLoaded = false;
}

std::string Texture::getFilePath() const
{
	return _filePath;
}

int Texture::getWidth() const
{
	return _width;
}

int Texture::getHeight() const
{
	return _height;
}

int Texture::getBytesPerPixel() const
{
	return _bytesPerPixel;
}

int Texture::getNumTextureImageUnits()
{
	static std::once_flag queryOnceFlag;
	static int maxTextureUnits;
	std::call_once(queryOnceFlag, []() {glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits); });

	return maxTextureUnits;
}

bool Texture::isLoadedCheck() const
{
	if (!_isLoaded)
	{
		std::cout << "Attempting to access non loaded texture!" << std::endl;
		return false;
	}

	return true;
}
