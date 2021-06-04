
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: gameManager.h
// Description	: Class to add objects to world space
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
//Implementation



#include "gameManager.h"
#include <time.h>




//Update function
void GameManager::Update(float dt)
{
	srand((unsigned int)time(NULL));
	unsigned int seed = 0;			//get a random seed through for enemy function
	int s = rand() % 750;
	seed += s;



	

}

//constructor
GameManager::GameManager(Camera* cam, GameObject* pl)
{
	c = cam;
	p = pl;
}