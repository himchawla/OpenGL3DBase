#pragma once
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <map>
#include <string>

#include "Camera.h"
#include "Terrain.h"

class OpenGLWindow
{
public:

	enum inputState
	{
		Input_Up,
		Input_UpFirst,
		Input_Down,
		Input_DownFirst,
	};
	static inputState KeyState[255];

	
	OpenGLWindow();
	bool createOpenGLWindow(int argc, char** argv);

	
	
	void runApp();

	void initializeScene();
	void renderScene();
	void releaseScene();
	void updateScene();
	void handleInput();
	bool keyPressed(int keyCode);
	bool keyPressedOnce(int keyCode);

	static void keyboardDown(unsigned char key, int x, int y)
	{
		KeyState[key] = Input_Down;
	}

	//Keyboard Up function
	static void keyboardUp(unsigned char key, int x, int y)
	{
		KeyState[key] = Input_Up;
	}

	void closeWindow(bool hasErrorOccured = false);

	bool hasErrorOccured();

private:

	Terrain terrain;
	bool _keyWasPressed[512];
	bool _hasErrorOccured = false;
	float _timeDelta;
	float _lastFrameTime;
	float _nextFPS = 0.0f;
	float _lastFrameTimeFPS;
	float _FPS;
	float currentAngle = 0.0f;
	Camera* camera;
	Object cube1;
	Object cube2;
	Object star;

	Object quad;
	VertexBufferObject shapesVBO;
	VertexBufferObject texCoordsVBO;
};
