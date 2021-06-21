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

	void mousePassive(int x, int y)
	{
		mouseX = (2.0f * x) / (float)1000;
		mouseY = 1.0f - (2.0f * y) / (float)1000;
	}



	bool updateMousePicking() {
		//screen pos
		glm::vec2 normalizedScreenPos = glm::vec2(mouseX, mouseY);
		//screenpos to Proj Space
		glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x,
			normalizedScreenPos.y, -1.0f, 1.0f);
		//Proj Space to eye space
		glm::mat4 invProjMat = glm::inverse(camera ->getprojectionMatrix());
		glm::vec4 eyeCoords = invProjMat * clipCoords;
		eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
		//Mouse Picking
			//eyespace to world space
			glm::mat4 invViewMat = glm::inverse(camera->getViewMatrix());
		glm::vec4 rayWorld = invViewMat * eyeCoords;
		rayDirection = glm::normalize(glm::vec3(rayWorld));


		float radius = 1.0f;
		glm::vec3 v = terrain->renderedPosition - camera->getPosition();
		float a = glm::dot(rayDirection, rayDirection);
		float b = 2 * glm::dot(v, rayDirection);
		float c = glm::dot(v, v) - radius * radius;
		float d = b * b - 4 * a * c;
		if (d > 0) {
			float x1 = (-b - sqrt(d)) / 2;
			float x2 = (-b + sqrt(d)) / 2;
			if (x1 >= 0 && x2 >= 0) return true; // intersects
			if (x1 < 0 && x2 >= 0) return true; // intersects
		}
		else if (d <= 0) {
			return false;// no intersection
		}
		//add code to check
		// intersection with other objects
	}


private:


	glm::vec3 rayDirection;
	float mouseY;
	float mouseX;


	

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
