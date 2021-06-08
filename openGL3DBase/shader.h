#pragma once
#include <glew.h>
#include <string>
#include <vector>
class shader
{
public:
	bool loadShaderFromFile(const std::string& _name, GLenum _type);
	bool isLoaded() const;
	void deleteShader();

	GLuint getShaderID() const;
	GLenum getShaderType() const;

private:
	bool getLinesFromFile(const std::string _name, std::vector<std::string>& _result, bool _isIncludedFile = false);
	GLuint m_shaderID;
	GLenum m_shaderType;
	bool m_isLoaded = false;
};

