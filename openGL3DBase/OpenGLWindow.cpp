#include "OpenGLWindow.h"

#include <iostream>
#include <math.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#include "Object.h"
#include "Object3D.h"
#include "ShaderProgram.h"
#include "VertexBufferObject.h"


shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
Object cube1;
Object cube2;
VertexBufferObject shapesVBO;
VertexBufferObject texCoordsVBO;
GLuint mainVAO;

OpenGLWindow::OpenGLWindow()
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

OpenGLWindow::inputState OpenGLWindow::KeyState[255];


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
	
	
	
	
	shapesVBO.addUpload(Object3D::cubeVertices, 0);

	texCoordsVBO.addUpload(Object3D::cubeFaceColors, 1, 6);


	cube1.init(vertexShader, fragmentShader, 0, shapesVBO, texCoordsVBO, camera);
	cube1.setTexture("Resources/Texture/Rayman.png");
	cube2.init(vertexShader, fragmentShader, 0, shapesVBO, texCoordsVBO, camera);
	cube2.setTexture("Resources/Texture/Rayman.png");
	cube1.transform.position = glm::vec3();
	cube2.transform.position = glm::vec3(4.0f, 0.0f, 0.0f);
	
	
}

void OpenGLWindow::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	
	cube1.Render();
	cube2.Render();
	
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
	camera->update([this](int keyCode) {return this->keyPressed(keyCode); }, [this](float f) {return sin(f); });

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
	cube1.transform.rotation = glm::vec3(0.0005f, 0.0005f, 0.0005f);
	cube1.Update();
	cube2.Update();

	
}

void OpenGLWindow::handleInput()
{
	
}

bool OpenGLWindow::keyPressed(int keyCode)
{
	
	return OpenGLWindow::KeyState[keyCode] == Input_Down;
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





