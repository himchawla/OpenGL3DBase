#pragma once
#include"Camera.h"
#include"ShaderLoader.h"

#include<string>

#include <glew.h>
#include <freeglut.h>

#include<stb_image.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>


class Cubemap
{
private:
	GLuint texID;


	GLuint VAO;
	GLuint VBO;
	GLuint EBO;



public:
	GLuint getTexID();
	glm::vec3 ObjPosition;
	glm::vec3 objRotationAngle;
	glm::vec3 objScale;

	GLuint program;
	Camera* camera;

	GLuint textures[6];
	
	glm::mat4 PVM;
	


	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Cubemap(std::string path, Camera* camera);


	//Loads the texture for the skybox
	void loadTextures();

	//Update function
	void Update();

	// Draws the model, and thus all its meshes
	void Render();

private:

};

