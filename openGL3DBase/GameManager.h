
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: gameManager.h
// Description	: gameManager
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
//Local Includes



#pragma once

#include<glew.h>
#include<freeglut.h>

#include<vector>
#include<string>
#include "GameObject.h"



//Convert string to int, used for input
constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

enum inputState
{
	Input_Up,
	Input_UpFirst,
	Input_Down,
	Input_DownFirst,
};
static inputState KeyState[255];  



class GameManager
{

private:

	float inputDelay = 0.2f;
	Camera* c;
	GameObject* p;

public:

	bool stencil = true, scissor = false, wireframe = false, backface = true, cubemap = false;

	//Constructor
	GameManager(Camera* cam, GameObject* pl);


	int currentFrame;

	struct input
	{
		unsigned char key;
		std::string name = "";
	};

	std::vector<input> inputArray;


	void Update(float dt);

	//Function for adding input
	bool addInput(unsigned char key, std::string name)
	{
		for (auto& i : inputArray)
		{
			if (i.name == name)
			{
				return false;
			}
		}

		input newInput = { key, name };
		inputArray.push_back(newInput);
		return true;
	}

	//add input overload, calls the add input function to add input
	void addInput()
	{
		addInput('s', "stencil");
		addInput('z', "wireframe");
		addInput('b', "backface");
		addInput('t', "scissor");
		addInput('c', "cubemap");
		addInput('r', "reset");
		

		addInput('-', "backward");
		addInput('=', "forward");

	}


	//Keyboard Down function
	static void keyboardDown(unsigned char key, int X, int Y)
	{
		KeyState[key] = Input_Down;
	}

	//Keyboard Up function
	static void keyboardUp(unsigned char key, int X, int Y)
	{
		KeyState[key] = Input_Up;
	}





	//compares the input array with the key pressed
	std::vector<std::string> isKeyPressed(unsigned char key)
	{
		std::vector<std::string> returnStuff;
		for (auto& i : inputArray)
		{
			if (KeyState[i.key] == Input_Down)
				returnStuff.push_back(i.name);
		}
		return returnStuff;
	}


	//is key down, takes key as param
	bool isKeyDown(unsigned char key)
	{
		if (KeyState[key] == Input_Down)
			return true;
	}


	//Movement function for the player
	void keyboardMove(GameObject& obj, float dt)
	{
		std::vector<std::string> tempInput;
		tempInput = isKeyPressed('0');
		
		inputDelay -= dt;

		for (auto& i : tempInput)
		{

			switch (str2int(i.c_str()))
			{
			case str2int("cubemap"):
			{
				if (inputDelay < 0.0f)
				{
					cubemap = !cubemap;
					inputDelay = 0.2f;
				}
			}break;
			
			

			default:
			{
			}

			}
		}
	}






};
