#pragma once
#include "shaderProgram.h"
#include "VertexBufferObject.h"

class Object2D
{
public:
	bool init()
	{
		vertexShader.loadShaderFromFile("Resources/Shaders/vertex.vs", GL_VERTEX_SHADER);
		fragmentShader.loadShaderFromFile("Resources/Shaders/fragment.fs", GL_FRAGMENT_SHADER);

		if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
		{
			return false;
		}

		mainProgram.createProgram();
		mainProgram.addShaderToProgram(vertexShader);
		mainProgram.addShaderToProgram(fragmentShader);

		if (!mainProgram.linkProgram())
		{
			
			return false;
		}

		glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
		glBindVertexArray(mainVAO);

		glm::vec3 vTriangle[] = { glm::vec3(-0.4f, 0.1f, 0.0f), glm::vec3(0.4f, 0.1f, 0.0f), glm::vec3(0.0f, 0.7f, 0.0f) };
		glm::vec3 vTriangleColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f) };

		glm::vec3 vCube[] = { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f) };
		glm::vec3 vCubeColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f) };

		shapesVBO.createVBO();
		shapesVBO.bindVBO();
		shapesVBO.addData(vTriangle);

		shapesVBO.addData(vCube);
		shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		colorsVBO.createVBO();
		colorsVBO.bindVBO();
		colorsVBO.addData(vTriangleColors, sizeof(glm::vec3) * 3);
		colorsVBO.addData(vCubeColors, sizeof(glm::vec3) * 4);
		colorsVBO.uploadDataToGPU(GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		return true;
	}
private:
	GLuint mainVAO;

	shader vertexShader, fragmentShader;
	ShaderProgram mainProgram;
	VertexBufferObject shapesVBO;
	VertexBufferObject colorsVBO;

};

