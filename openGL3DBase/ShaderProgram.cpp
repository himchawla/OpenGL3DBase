// STL
#include <iostream>

// Project
#include "shaderProgram.h"

ShaderProgram::~ShaderProgram()
{
    //deleteProgram();
}

void ShaderProgram::createProgram()
{
    m_shaderProgramID = glCreateProgram();
}

bool ShaderProgram::addShaderToProgram(const shader& shader)
{
    if (!shader.isLoaded())
        return false;

    glAttachShader(m_shaderProgramID, shader.getShaderID());

    return true;
}

bool ShaderProgram::linkProgram()
{
    if (m_isLinked) {
        return true;
    }

    glLinkProgram(m_shaderProgramID);
    int linkStatus;
    glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus);
    m_isLinked = linkStatus == GL_TRUE;

    if (!m_isLinked)
    {
        char infoLogBuffer[2048];
        int logLength;
        glGetProgramInfoLog(m_shaderProgramID, 2048, &logLength, infoLogBuffer);

        std::cout << "Error! Shader program wasn't linked! The linker returned: " << std::endl << std::endl << infoLogBuffer << std::endl;

        return false;
    }

    return m_isLinked;
}

void ShaderProgram::useProgram() 
{
    if (m_isLinked) {
        glUseProgram(m_shaderProgramID);
    }
}

void ShaderProgram::unUseProgram()
{
    glUseProgram(0);
}

void ShaderProgram::deleteProgram()
{
    if (!m_isLinked) {
        return;
    }

    glDeleteProgram(m_shaderProgramID);
    m_isLinked = false;
}

GLuint ShaderProgram::getShaderProgramID() const
{
    return m_shaderProgramID;
}

Uniform& ShaderProgram::operator[](const std::string& varName)
{
    if (m_uniform.count(varName) == 0)
    {
        m_uniform[varName] = Uniform(varName, this);
    }
    m_uniform[varName].SetStuff(varName, this);
    return m_uniform[varName];
}

