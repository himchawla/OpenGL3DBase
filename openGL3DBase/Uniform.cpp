#include "Uniform.h"

#include <iostream>

#include "ShaderProgram.h"

Uniform::Uniform()
{
	
}

Uniform::Uniform(const std::string& _name, ShaderProgram* _shaderProgram)
	:m_shaderProgram(_shaderProgram),
	m_name(_name)
{
	m_location = glGetUniformLocation(m_shaderProgram->getShaderProgramID(), _name.c_str());
	if (m_location == -1)
	{
		std::cout << "WARNING: uniform with name " << _name << " does not exist, setting it will fail!" << std::endl;
	}
}

void Uniform::SetStuff(const std::string& _name, ShaderProgram* _shaderProgram)
	
{
	m_shaderProgram = _shaderProgram;
	m_name = _name;
	m_location = glGetUniformLocation(m_shaderProgram->getShaderProgramID(), _name.c_str());
	if (m_location == -1)
	{
		std::cout << "WARNING: uniform with name " << _name << " does not exist, setting it will fail!" << std::endl;
	}
}
Uniform& Uniform::operator=(const glm::vec2& _vector2D)
{
	set(_vector2D);
	return *this;
}

void Uniform::set(const glm::vec2* _vectors2D, int _count)
{
	glUniform2fv(m_location, _count, (GLfloat*)_vectors2D);
}


void Uniform::set(const glm::vec2& _vector2D)
{
	glUniform2fv(m_location, 1, (GLfloat*)&_vector2D);
}

Uniform& Uniform::operator=(const glm::vec3& _vector3D)
{
	set(_vector3D);
	return *this;
}

void Uniform::set(const glm::vec3& _vector3D)
{
	glUniform3fv(m_location, 1, (GLfloat*)&_vector3D);
}

void Uniform::set(const glm::vec3* _vectors3D, int _count)
{
	glUniform3fv(m_location, _count, (GLfloat*)_vectors3D);
}

Uniform& Uniform::operator=(const glm::vec4& _vector4D)
{
	set(_vector4D);
	return *this;
}

void Uniform::set(const glm::vec4& _vector4D)
{
	glUniform4fv(m_location, 1, (GLfloat*)&_vector4D);
}

void Uniform::set(const glm::vec4* _vectors4D, int _count)
{
	glUniform4fv(m_location, _count, (GLfloat*)_vectors4D);
}

Uniform& Uniform::operator=(const float _floatValue)
{
	set(_floatValue);
	return *this;
}

Uniform& Uniform::operator=(const std::vector<float>& _floatValues)
{
	set(_floatValues.data(), _floatValues.size());
	return *this;
}

void Uniform::set(const float _floatValue)
{
	glUniform1fv(m_location, 1, (GLfloat*)&_floatValue);
}

void Uniform::set(const float* _floatValues, int _count)
{
	glUniform1fv(m_location, _count, (GLfloat*)_floatValues);
}

// Family of functions setting integer uniforms

Uniform& Uniform::operator=(const int _integerValue)
{
	set(_integerValue);
	return *this;
}

void Uniform::set(const int _integerValue)
{
	glUniform1iv(m_location, 1, (GLint*)&_integerValue);
}

void Uniform::set(const int* _integerValues, int _count)
{
	glUniform1iv(m_location, _count, (GLint*)_integerValues);
}

Uniform& Uniform::operator=(const bool _boolValue)
{
	set(_boolValue);
	return *this;
}

void Uniform::set(const bool _boolValue)
{
	set(_boolValue ? 1 : 0);
}

void Uniform::set(const bool* _boolValue, int _count)
{
	std::vector<int> integerValues;
	for (auto i = 0; i < _count; i++)
	{
		integerValues[i] = _boolValue[i] ? 1 : 0;
	}

	set(integerValues.data(), _count);
}

// Family of functions setting 3x3 matrices uniforms

Uniform& Uniform::operator=(const glm::mat3& _matrix)
{
	set(_matrix);
	return *this;
}

void Uniform::set(const glm::mat3& _matrix)
{
	glUniformMatrix3fv(m_location, 1, false, (GLfloat*)&_matrix);
}

void Uniform::set(const glm::mat3* _matrices, int _count)
{
	glUniformMatrix3fv(m_location, _count, false, (GLfloat*)_matrices);
}

// Family of functions setting 4x4 matrices uniforms

Uniform& Uniform::operator=(const glm::mat4& _matrix)
{
	set(_matrix);
	return *this;
}

void Uniform::set(const glm::mat4& _matrix)
{
	glUniformMatrix4fv(m_location, 1, false, (GLfloat*)&_matrix);
}

void Uniform::set(const glm::mat4* _matrices, int _count)
{
	glUniformMatrix4fv(m_location, _count, false, (GLfloat*)_matrices);
}