#pragma once
#include <map>

#include "shader.h"
#include "Uniform.h"

class ShaderProgram
{
public:
	~ShaderProgram();
	void createProgram();
	bool addShaderToProgram(const shader& _shader);
	bool linkProgram();
	void useProgram();
	void unUseProgram();
	void deleteProgram();

	GLuint getShaderProgramID() const;

	Uniform& operator[](const std::string& _varName);

	GLuint m_shaderProgramID;
	bool m_isLinked = false;
	std::map<std::string, Uniform> m_uniform;
};

