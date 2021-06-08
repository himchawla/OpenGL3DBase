#pragma once

// STL
#include <vector>
#include <string>

// GLM

#include <glm.hpp>


class ShaderProgram;


class Uniform
{
public:
    Uniform();
    Uniform(const std::string& name, ShaderProgram* shaderProgram);
    void SetStuff(const std::string& _name, ShaderProgram* _shaderProgram);

    Uniform& operator=(const glm::vec2& _vector2D);
    void set(const glm::vec2& _vector2D);
    void set(const glm::vec2* _vectors2D, int count = 1);

    Uniform& operator=(const glm::vec3& _vector3D);
    void set(const glm::vec3& _vector3D);
    void set(const glm::vec3* _vectors3D, int _count = 1);

    Uniform& operator=(const glm::vec4& _vector4D);
    void set(const glm::vec4& _vector4D);
    void set(const glm::vec4* _vectors4D, int _count = 1);

    Uniform& operator=(const float _floatValue);
    Uniform& operator=(const std::vector<float>& _floatValues);
    void set(const float _floatValue);
    void set(const float* _floatValues, int _count = 1);

    Uniform& operator=(const int _integerValue);
    void set(const int _integerValue);
    void set(const int* _integerValues, int _count = 1);

    Uniform& operator=(const bool _boolValue);
    void set(const bool _boolValue);
    void set(const bool* _boolValue, int _count = 1);

    Uniform& operator=(const glm::mat3& _matrix);
    void set(const glm::mat3& _matrix);
    void set(const glm::mat3* _matrices, int _count = 1);

    Uniform& operator=(const glm::mat4& _matrix);
    void set(const glm::mat4& _matrix);
    void set(const glm::mat4* _matrices, int _count = 1);

private:
    ShaderProgram* m_shaderProgram; 

    std::string m_name; 
    int m_location; 
};
