#include "OpenGLWindow.h"

#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#include "ShaderProgram.h"
#include "VertexBufferObject.h"


shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;
VertexBufferObject colorsVBO;

GLuint mainVAO;

OpenGLWindow::OpenGLWindow()
{
}

bool OpenGLWindow::createOpenGLWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("OpenGL Project");
	_lastFrameTime = (float)glutGet(GLUT_ELAPSED_TIME);
	

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation has failed. Terminating. ";
		return 0;
	}

	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);
	return true;
}

void OpenGLWindow::runApp()
{
	initializeScene();
}


Camera camera(glm::vec3(0.0f, 8.0f, 20.0f), glm::vec3(0.0f, 8.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float rotationAngleRad;
void OpenGLWindow::initializeScene()
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	vertexShader.loadShaderFromFile("Resources/Shaders/vertex.vs", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders/fragment.fs", GL_FRAGMENT_SHADER);

	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
	{
		closeWindow(true);
		return;
	}

	mainProgram.createProgram();
	mainProgram.addShaderToProgram(vertexShader);
	mainProgram.addShaderToProgram(fragmentShader);

	if (!mainProgram.linkProgram())
	{
		closeWindow(true);
		return;
	}

	glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
	glBindVertexArray(mainVAO);

	
	glm::vec3 vCube[] = { // Front face
	glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
	// Back face
	glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
	// Left face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Right face
	glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
	// Top face
	glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
	// Bottom face
	glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
	};
	glm::vec3 vCubeColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f) };

	shapesVBO.createVBO();
	shapesVBO.bindVBO();
	
	shapesVBO.addData(vCube);
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	colorsVBO.createVBO();
	colorsVBO.bindVBO();
	colorsVBO.addData(vCubeColors, sizeof(glm::vec3) * 4);
	colorsVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	
	
}

void OpenGLWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	//glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), // Eye position
	//	glm::vec3(0.0f, 0.0f, 0.0f),  // Viewpoint position
	//	glm::vec3(0.0f, 1.0f, 0.0f)); // Up vector

	//glm::mat4 projectionMatrix = glm::perspective(
	//	45.0f, // field of view angle (in degrees)
	//	float(1000) / float(1000), // aspect ratio
	//	0.5f, // near plane distance
	//	1000.0f); // far plane distance
	//
	//// Render rotating cube in the middle
	//auto modelMatrixCube = glm::mat4(1.0);
	//modelMatrixCube = glm::rotate(modelMatrixCube, currentAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	//modelMatrixCube = glm::rotate(modelMatrixCube, currentAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrixCube = glm::rotate(modelMatrixCube, currentAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	//modelMatrixCube = glm::scale(modelMatrixCube, glm::vec3(5.0f, 5.0f, 5.0f));

	//
	//
	//mainProgram["PVM"] = projectionMatrix * viewMatrix * modelMatrixCube;
	glDrawArrays(GL_TRIANGLES, 0, 36);

	currentAngle += 10.0f;

	glutSwapBuffers();

}

void OpenGLWindow::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();

	glDeleteVertexArrays(1, &mainVAO);
}

void OpenGLWindow::updateScene()
{
	
	const auto currentTime = (float)glutGet(GLUT_ELAPSED_TIME);
	_timeDelta = currentTime - _lastFrameTime;
	_lastFrameTime = currentTime;
	_nextFPS++;

	if (currentTime - _lastFrameTimeFPS > 1.0)
	{
		_lastFrameTimeFPS = currentTime;
		_FPS = _nextFPS;
		_nextFPS = 0;
	}
}

void OpenGLWindow::handleInput()
{
	
}

bool OpenGLWindow::keyPressed(int keyCode)
{
	return OpenGLWindow::KeyState[keyCode] > 0;
}

bool OpenGLWindow::keyPressedOnce(int keyCode)
{
	return true;
}


void OpenGLWindow::closeWindow(bool hasErrorOccured)
{
}

bool OpenGLWindow::hasErrorOccured()
{
	return true;
}





