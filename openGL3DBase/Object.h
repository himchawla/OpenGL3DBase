#pragma once
#include "shaderProgram.h"
#include "VertexBufferObject.h"
#include <gtc/matrix_transform.hpp>

#include "Camera.h"
#include "sampler.h"
#include "Texture.h"

class Object
{
public:
	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	}transform;


	bool init(std::string _vertPath, std::string _fragPath);
	bool init(shader _vertex, shader _frag, GLuint _VAO, VertexBufferObject _shapes, VertexBufferObject _colors,
	          Camera* _cam);
	
	template<typename T>
	void setVertData(const T& _data);

	template<typename T>
	void setFragData(const T& _data);

	Object();

	void Update();
	void Render();

	void setTexture(std::string _texture);

private:
	glm::mat4 modelMatrix;
	Camera* camera;

	GLuint mainVAO;

	shader vertexShader, fragmentShader;
	ShaderProgram program;
	VertexBufferObject shapesVBO;
	VertexBufferObject colorsVBO;

	Texture texture;

};
