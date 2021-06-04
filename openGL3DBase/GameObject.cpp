//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: object.cpp
// Description	: Class to make shapes and add texturing
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
//Implementation


#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include "gameManager.h"





std::string GameObject::GetName() const
{
	return m_name;
}

void GameObject::SetName(std::string _name)
{
	m_name = _name;
}

auto GameObject::SetTransform(const glm::vec3 _objPos, const glm::vec3 _objRotAngle, const glm::vec3 _objSc) -> void
{
	transform.position = _objPos;
	eulerAngles(transform.rotation) = _objRotAngle;
	objScale = _objSc;
}



