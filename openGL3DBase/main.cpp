#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include <string>
#include<vector>
#include<time.h>

//Local includes
#include "ShaderLoader.h"
#include "GameObject.h"
#include"GameManager.h"

#include "Model.h"
#include"Cubemap.h"

float index = 0;
float dt;				//the infamous delta time
float old_t;
float score;
void resetTroops();			//function forward declaration
float angle = 0.0f;
glm::vec2 pos(0.0, 0.0);
GLuint tex;

//Region object declaration
#pragma region Objects

Cubemap* cubemap;


GameObject* cube;
GameManager* gameManager;
Camera* cam;

#pragma endregion


float mouseY;
float mouseX;
glm::vec3 rayDirection;



//Shutdown function
void Shutdown()
{
}

void mouseMove(int x, int y)
{
	mouseX = -1.0 + 2.0 * x / 1000;
	mouseY = 1.0 - 2.0 * y / 1000;

	angle = 90 + atan2(pos.y - mouseY, pos.x - mouseX) * 180 / 3.1415926;

	//std::cout << mouseX << ", " << mouseY << std::endl;
	//std::cout << x << ", " << y << std::endl;
	//std::cout << angle << std::endl;
}

bool updateMousePicking() {
	return true;
}


//Render Function
void Render()
{
		//cubemap->Render();
		cube->Render();
		

	glutSwapBuffers();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


//update function, used to call update functions of other classes
void Update()
{

	int t;
	/* Delta time in seconds. */
	t = glutGet(GLUT_ELAPSED_TIME);
	dt = (t - old_t) / 1000.0f;
	old_t = (float)t;

	glutPostRedisplay();
	

	//updateMousePicking();
	cubemap->Update();
	gameManager->keyboardMove(*cube, dt);
	cam->Update(dt);

	//cube->updateObject(dt);

}




int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("OpenGL Project");

	old_t = (float)glutGet(GLUT_ELAPSED_TIME);	
	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation has failed. Terminating. ";
		return 0;
	}

	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

//Region initialize objects
#pragma region InitObjects
	
	cam = new Camera;
	cubemap = new Cubemap("", cam);
	cubemap->loadTextures();

	cube = new GameObject("Resources/Models/Cube/cube.obj", cam);

	

	gameManager = new GameManager(cam, cube);
	gameManager->addInput();
	
#pragma endregion



//Region set positions
#pragma region Positions

	

#pragma endregion

	//stencil->setScale(-0.11f, 1.11f);
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	//glutCloseFunc(Shutdown);

	glutKeyboardFunc(GameManager::keyboardDown);
	glutKeyboardUpFunc(GameManager::keyboardUp);

	glutMotionFunc(mouseMove);

	glutIgnoreKeyRepeat(GL_TRUE);

	glutMainLoop();

	return 0;
}