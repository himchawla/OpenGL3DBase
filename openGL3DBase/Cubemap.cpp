
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2019 Media Design School
//
// File Name 	: Cubemap.cpp
// Description	: Skybox
// Author 		: Himanshu Chawla
// Mail 		: Himanshu.Cha8420
//
#include "Cubemap.h"



GLuint Indices[] =
{
   0, 1, 2,		//Front
   0, 2, 3,		//Front

   7, 6, 5,		//Back
   7, 5, 4,		//Back

   8, 9, 10,	//Left
   8, 10, 11,	//Left

   12, 13, 14,	//Right
   12, 14, 15,	//Right

   16, 17, 18,	//Top
   16, 18, 19,	//Top

   20, 21, 22,	//Bottom
   20, 22, 23,	//Bottom
};


GLfloat Vertices[] =
{
	//Position						//Color						//Texture Coordinates
	//Front
	/*  0 */		-0.5f,	0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,0.0f,		/*  00  */
	/*  1 */		-0.5f,	-0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,1.0f,		/*  01  */
	/*  2 */  		 0.5f,	-0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,1.0f,		/*  02  */
	/*  3 */		 0.5f,	 0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,0.0f,		/*  03  */
	//Back
	/*  4 */		-0.5f,	0.5f,  -0.5f,			0.0f,1.0f,0.0f,				1.0f,0.0f,		/*  04  */
	/*  5 */		-0.5f,	-0.5f, -0.5f,			0.0f,1.0f,0.0f,				1.0f,1.0f,		/*  05  */
	/*  6 */  		 0.5f,	-0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  06  */
	/*  7 */		 0.5f,	 0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  07  */

	/*  4 */		-0.5f,	0.5f,  -0.5f,			0.0f,1.0f,0.0f,				1.0f,0.0f,		/*  08  */
	/*  5 */		-0.5f,	-0.5f, -0.5f,			0.0f,1.0f,0.0f,				1.0f,1.0f,		/*  09  */
	/*  1 */		-0.5f,	-0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,1.0f,		/*  10  */
	/*  0 */		-0.5f,	0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,0.0f,		/*  11  */

	/*  3 */		 0.5f,	 0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,0.0f,		/*  12  */
	/*  2 */  		 0.5f,	-0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,1.0f,		/*  13  */
	/*  6 */  		 0.5f,	-0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  14  */
	/*  7 */		 0.5f,	 0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  15  */

	/*  4 */		-0.5f,	0.5f,  -0.5f,			0.0f,1.0f,0.0f,				1.0f,0.0f,		/*  16  */
	/*  0 */		-0.5f,	0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,0.0f,		/*  17  */
	/*  3 */		 0.5f,	 0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,0.0f,		/*  18  */
	/*  7 */		 0.5f,	 0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  19  */

	/*  1 */		-0.5f,	-0.5f,	0.5f,			0.0f,1.0f,0.0f,				0.0f,1.0f,		/*  20  */
	/*  5 */		-0.5f,	-0.5f, -0.5f,			0.0f,1.0f,0.0f,				1.0f,1.0f,		/*  21  */
	/*  6 */  		 0.5f,	-0.5f, -0.5f,			0.0f,1.0f,1.0f,				0.0f,0.0f,		/*  22  */
	/*  2 */  		 0.5f,	-0.5f,	0.5f,			0.0f,1.0f,1.0f,				1.0f,1.0f,		/*  23  */
};



GLuint Cubemap::getTexID()
{
	return texID;
}


//Constructor
Cubemap::Cubemap(std::string path, Camera* camera)
{
	// EDIT
	this->program = ShaderLoader::CreateProgram("Resources/Shaders/cubemap.vs", "Resources/Shaders/cubemap.fs");
	this->camera = camera;
	// EDIT END

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

void Cubemap::loadTextures()
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	int width, height, components;
	unsigned char* image;
	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Texture/CubeMap/";
		fullPathName.append(std::to_string(i + 1) + ".jpg");
		image = stbi_load(fullPathName.c_str(), &width, &height, &components, 0);
		GLint LoadedComponents = (components == 4) ? GL_RGBA : GL_RGB;
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
				);

			stbi_image_free(image);
		}

		else
		{
			std::cout << "Cubemap tex failed to load at path: " << i << std::endl;
			stbi_image_free(image);
		}

	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//Update function
void Cubemap::Update()
{

	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(2000.0f, -2000.0f, 2000.0f));
	PVM = camera->getPVM() * model;
}

//Render function
void Cubemap::Render()
{
	glUseProgram(program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glUniform1i(glGetUniformLocation(program, "cubeSampler"), 0);
	
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, glm::value_ptr(PVM));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}