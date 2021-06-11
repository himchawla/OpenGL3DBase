#include <iostream>
#include <gtc/quaternion.hpp>

#include "OpenGLWindow.h"
OpenGLWindow win;


glm::vec2 pos(0.0, 0.0);

float mouseY;
float mouseX;

float angle = 0.0f;

void mouseMove(int x, int y)
{
	mouseX = -1.0 + 2.0 * x / 1000;
	mouseY = 1.0 - 2.0 * y / 1000;

	angle = 90 + atan2(pos.y - mouseY, pos.x - mouseX) * 180 / 3.1415926;

	//std::cout << mouseX << ", " << mouseY << std::endl;
	//std::cout << x << ", " << y << std::endl;
	//std::cout << angle << std::endl;
}


void Update()
{
	win.updateScene();
	win.renderScene();
}

void Render()
{



}

void main(int argc, char** argv)
{
	
	if (!win.createOpenGLWindow(argc, argv))
	{
		std::cout << "Failed to createe window";
		return;
	}
	win.initializeScene();

	glutIdleFunc(Update);
	glutDisplayFunc(Render);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(OpenGLWindow::keyboardDown);
	glutKeyboardUpFunc(OpenGLWindow::keyboardUp);
	glutMainLoop();
}
