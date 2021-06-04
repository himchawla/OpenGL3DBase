
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: Camera.cpp
// Description	: Class to add objects to world space
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
//Implementation

#include "Camera.h"
#include<iostream>	

glm::mat4 Camera::getProjectionMat()
{
	return projectionMat;
}

glm::mat4 Camera::getViewMat()
{
	return viewMat;
} //Constructor
Camera::Camera()
{
	screenHeight = 800;
	screenWidth = 800;
	cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
	cameraLookDir = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUpDir = glm::vec3(0.0f, -1.0f, 0.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	viewMat = glm::lookAt(cameraPos, target, cameraUpDir);
	projectionMat = glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f);
	timeElapsed = 0.0f;
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
}


void Camera::Update(float deltaTime)
{
	timeElapsed += deltaTime;

	

	glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);
	viewMat = glm::lookAt(cameraPos, target, up);
}



//Returns Product of Projection and View Matrix
glm::mat4 Camera::getMat()
{
	glm::mat4 resMat = projectionMat * viewMat;
	return resMat;
}


//Returns Resultant Matrix
glm::mat4 Camera::project(glm::mat4 modelMat)
{
	glm::mat4 finalMat = getMat() * modelMat;
	return	  finalMat;
}

glm::mat4 Camera::getPVM()
{
	glm::mat4 tempView = glm::lookAt(cameraPos, glm::vec3(0.0f,0.0f,0.0f), cameraUpDir);
	glm::mat4 resMat = projectionMat * tempView;

	return resMat;
}