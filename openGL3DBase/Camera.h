
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: Camera.h
// Description	: Class to add objects to world space
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Chawla@mds.ac.nz
//

#pragma once

#include <glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<glew.h>
#include<freeglut.h>


class Camera
{
private:
	glm::vec3 cameraLookDir;
	glm::vec3 cameraUpDir;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	float screenWidth;
	float screenHeight;
	float timeElapsed;
	float Rotation = 180.0f;

public:
	glm::mat4 getProjectionMat();

	glm::mat4 getViewMat();

	//Constructor
	Camera();

	//Update CameraUpDir
	
	//Update function
	void Update(float deltaTime);

	//returns projection * view mat
	glm::mat4 getMat();

	//Projection matrix
	glm::mat4 project(glm::mat4 modelMat);

	//returns PV matrix with no target, used for the skybox
	glm::mat4 getPVM();

	glm::vec3 target;
	glm::vec3 cameraFront;

	glm::vec3 cameraPos;



};

