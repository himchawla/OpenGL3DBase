#include "OpenGLWindow.h"

#include <iostream>
#include <math.h>

#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#include "Object3D.h"
#include "Object.h"
#include "ShaderProgram.h"
#include "Terrain.h"
#include "VertexBufferObject.h"


shader vertexShader, fragmentShader;
ShaderProgram mainProgram;

GLuint mainVAO;

OpenGLWindow::OpenGLWindow()
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	camera->setRotationSpeed(50.0f);
}

bool OpenGLWindow::createOpenGLWindow(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_GEOMETRY_VISUALIZE_NORMALS);
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

	vertexShader.loadShaderFromFile("Resources/Shaders/vertex.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders/fragment.frag", GL_FRAGMENT_SHADER);

	
	
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
	
	
	glm::vec3 vertex[] = {
		glm::vec3(0.5f,0.5f,0.5f)
	};
	
	shapesVBO.addUpload(Object3D::cubeVertices, 0);

	texCoordsVBO.addUpload(Object3D::cubeFaceColors, 1, 6);

	terrain.init(camera);
	terrain.setTexture("Resources/Texture/brick.png");
	cube1.init(vertexShader, fragmentShader, 0, shapesVBO, texCoordsVBO, camera, &terrain);
	cube1.setTexture("Resources/Texture/Rayman.png");
	cube2.init(vertexShader, fragmentShader, 0, shapesVBO, texCoordsVBO, camera, &terrain);
	cube2.setTexture("Resources/Texture/Rayman.png");
	cube1.transform.position = glm::vec3();
	cube2.transform.position = glm::vec3(4.0f, 0.0f, 0.0f);

	star.init("Geometery.vert", "Geometery.frag", "Geometery.geom", camera, 1);
	//star.setTexture("Resources/Texture/Red.png");
	
	camera->cube = &cube1;

	quad.initQuad("tess.vert", "tess.frag", camera, true);

	/*quad.GetVBO().addUpload(Object3D::quadVertices,0);
	quad.transform.scale = glm::vec3(1.0f);
	quad.setVerts(4);*/
	
	
	
}

void OpenGLWindow::renderScene()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	terrain.Render();
	
	glCullFace(GL_FRONT);

	cube1.Render();
	//cube2.Render();
	glDisable(GL_CULL_FACE);

	//quad.quadLOD();
	quad.RenderQuad();
	star.RenderGeom();
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
	cube1.Update(_timeDelta);
	cube1.move([this](int keyCode) {return this->keyPressed(keyCode); });
	//cube2.Update(_timeDelta);
	//cube1.velocity = glm::vec3(0.0f, -0.0008f, 0.0f);
	star.Update(_timeDelta);


	
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





