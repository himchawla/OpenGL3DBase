#include <iostream>
#include "OpenGLWindow.h"
OpenGLWindow win;
void Render()
{
	win.renderScene();
}

void Update()
{
	win.updateScene();
}
void main(int argc, char** argv)
{
	
	if (!win.createOpenGLWindow(argc, argv))
	{
		std::cout << "Failed to createe window";
		return;
	}
	win.initializeScene();

	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutKeyboardFunc(OpenGLWindow::keyboardDown);
	glutKeyboardUpFunc(OpenGLWindow::keyboardUp);
	glutMainLoop();
}
