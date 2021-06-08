
#include "shader.h"
#include "string_uttils.h"
#include "Headers.h"
// STL
#include <sstream>
#include <iostream>
#include <fstream>


bool shader::loadShaderFromFile(const std::string& _name, GLenum _type)
{
	std::vector<std::string> lines;

	if (!getLinesFromFile(_name, lines))
		return false;

	const char** sProgram = new const char* [lines.size()];
	FOR(i, ESZ(lines))	sProgram[i] = lines[i].c_str();

	m_shaderID = glCreateShader(_type);

	glShaderSource(m_shaderID, (GLsizei)ESZ(lines), sProgram, NULL);
	glCompileShader(m_shaderID);

	delete[] sProgram;

	int compilationStatus;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compilationStatus);

	if(compilationStatus == GL_FALSE)
	{
		char infoLogBuffer[2048];
		int logLength;
		glGetShaderInfoLog(m_shaderID, 2048, &logLength, infoLogBuffer);
		std::cout << "Error! Shader file " << _name << " wasn't compiled! The compiler returned:" << std::endl << std::endl << infoLogBuffer << std::endl;
		return false;
	}
	m_isLoaded = true;
	return true;

	
}

bool shader::isLoaded() const
{
	return m_isLoaded;
}

void shader::deleteShader()
{
	if(!m_isLoaded)	return;

	m_isLoaded = false;
	glDeleteShader(m_shaderID);
}

GLuint shader::getShaderID() const
{
	return m_shaderID;
}

GLenum shader::getShaderType() const
{
	return m_shaderType;
}

bool shader::getLinesFromFile(const std::string _name, std::vector<std::string>& _result, bool _isIncludedFile)
{
	std::ifstream file(_name);

	if(!file.good())
	{
		std::cout << "File " << _name << " not found!~";
		return false;
	}

	std::string startDir;
	char slash = '/';
	auto normFileName = string_utils::normalizeSlashes(_name, slash);

	size_t slashIndex = -1;

	RFOR(i, ESZ(_name))
	{
		if(_name[i] == slash)
		{
			slashIndex = i;
			slash = _name[i];
			break;
		}
	}
	startDir = _name.substr(0, slashIndex + 1);

	std::string line;
	bool isInsideIncludePart = false;

	while (std::getline(file, line))
	{
		line += "\n"; // getline does not keep newline character
		std::stringstream ss(line);
		std::string firstToken;
		ss >> firstToken;
		if (firstToken == "#include")
		{
			std::string includeFileName;
			ss >> includeFileName;
			if (includeFileName.size() > 0 && includeFileName[0] == '\"' && includeFileName[includeFileName.size() - 1] == '\"')
			{
				includeFileName = string_utils::normalizeSlashes(includeFileName.substr(1, includeFileName.size() - 2), slash);
				std::string directory = startDir;
				std::vector<std::string> subPaths = string_utils::split(includeFileName, slash);
				std::string sFinalFileName = "";
				for (const std::string& subPath : subPaths)
				{
					if (subPath == "..")
						directory = string_utils::upOneDirectory(directory, slash);
					else
					{
						if (sFinalFileName.size() > 0)
							sFinalFileName += slash;
						sFinalFileName += subPath;
					}
				}
				getLinesFromFile(directory + sFinalFileName, _result, true);
			}
		}
		else if (firstToken == "#include_part")
			isInsideIncludePart = true;
		else if (firstToken == "#definition_part")
			isInsideIncludePart = false;
		else if (!_isIncludedFile || (_isIncludedFile && isInsideIncludePart))
			_result.push_back(line);
	}
}




