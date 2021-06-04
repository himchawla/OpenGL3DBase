
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: object.h
// Description	: Class to make shapes and add texturing
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
//Local Includes


#pragma once

#include<glew.h>
#include<freeglut.h>
#include<iostream>
#include "ShaderLoader.h"
#include<vector>
#include "Camera.h"
#include"Model.h"


class GameObject:public Model
{
private:
	
	std::string m_name;
	
public:

	struct Transform
	{
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	}transform;
	
	//Constructor
	GameObject(std::string s, Camera* cam) : Model(s, cam)
	{
		const glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
		const glm::vec3 position = glm::vec3(0.5f, 0.5f, 0.0f);
		const float rotation = 180.0f;
		camera = cam;
		SetTransform(position, glm::vec3(0.0f, 0.0f, rotation), scale);
	}

	std::string name;

	//Overloaded constructor calls overloaded version of
	GameObject(std::string _path, Camera* _cam, bool _n) : Model(_path, _cam, _n)
	{
		const glm::vec3 scale = glm::vec3(0.1f, 0.1f, 0.1f);
		const glm::vec3 position = glm::vec3(0.5f, 0.5f, 0.0f);
		const float rotation = 180.0f;
		camera = _cam;
		SetTransform(position, glm::vec3(0.0f, 0.0f, rotation), scale);
	}

	

	
	//Movement function, also updates camera pos
	

	//abs movement
	void absMove(float x, float y);

	//get Speed
	float getSpeed();

	//get X position the player
	float getX();
	//get Y position of the player
	float getY();
	//get Z position of the player
	float getZ();

	//set Speed
	void setSpeed(float s);

	
	//void Render(int in, std::vector<GLuint> t);

	
	glm::mat4 finalMat;
	//void Render(int in);
	//returns name of the object
	std::string GetName() const;
	//sets the name of the object
	void SetName(std::string n);
	
	
	
	void SetTransform(glm::vec3 _objPos, glm::vec3 _objRotAngle, glm::vec3 _objSc);



	

	//object();
	~GameObject()
	{
	}

	

private:
};

