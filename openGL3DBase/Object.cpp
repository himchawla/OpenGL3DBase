#include "Object.h"

#include "Camera.h"
#include "Headers.h"
#include "Terrain.h"

void Object::move(const std::function<bool(int)>& keyInputFunc)
{
	if(keyInputFunc('w'))
	{
		transform.position.z += 0.1f;
	}
	if (keyInputFunc('s'))
	{
		transform.position.z -= 0.1f;
	}
	if (keyInputFunc('a'))
	{
		transform.position.x += 0.1f;
	}
	if (keyInputFunc('d'))
	{
		transform.position.x -= 0.1f;
	}
}

bool Object::init(std::string _vertPath, std::string _fragPath, Camera* _camera, bool genVAO)
{
	vertexShader.loadShaderFromFile("Resources/Shaders/" + _vertPath, GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders/" + _fragPath, GL_FRAGMENT_SHADER);

	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
	{
		return false;
	}
	program.createProgram();
	program.addShaderToProgram(vertexShader);
	program.addShaderToProgram(fragmentShader);

	if (!program.linkProgram())
	{
		return false;
	}
	if (genVAO)
	{
		glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
		glBindVertexArray(mainVAO);
	}
	camera = _camera;
	/*transform.position = (glm::vec3(6.0f, 1.0f, 0.0f));

	translate(modelMatrix, transform.position);*/

	return true;
}


bool Object::initQuad(std::string _vertPath, std::string _fragPath, Camera* _camera, bool genVAO)
{
	vertexShader.loadShaderFromFile("Resources/Shaders/" + _vertPath, GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders/" + _fragPath, GL_FRAGMENT_SHADER);
	tcsShader.loadShaderFromFile("Resources/Shaders/tess.tesc", GL_TESS_CONTROL_SHADER);
	tesShader.loadShaderFromFile("Resources/Shaders/tess.tese", GL_TESS_EVALUATION_SHADER);
	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded() || !tcsShader.isLoaded() || !tesShader.isLoaded())
	{
		return false;
	}
	program.createProgram();
	program.addShaderToProgram(vertexShader);
	program.addShaderToProgram(fragmentShader);
	program.addShaderToProgram(tcsShader);
	program.addShaderToProgram(tesShader);
	
	if (!program.linkProgram())
	{
		return false;
	}
	if (genVAO)
	{
		glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
		glBindVertexArray(mainVAO);
	}
	camera = _camera;

	GLfloat points[] = { -1.0f, -1.0f, 0.0f,
						  1.0f, -1.0f, 0.0f,
					   	  1.0f, 1.0f, 0.0f,
						 -1.0, 1.0, 0.0f ,
						  1.0f, 1.0f, 0.0f,
						 -1.0f, -1.0f, 0.0f, };

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	shapesVBO.addUpload(points, 0);
	transform.position = glm::vec3(200.0f, 5.0f, 0.0f);
	modelMatrix = glm::translate(modelMatrix, transform.position);
	transform.scale = glm::vec3(75.0f);
	modelMatrix = glm::scale(modelMatrix, transform.scale);
	transform.rotation.x = glm::radians(90.0f);
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.x, glm::vec3(1.0f,0.0f,0.0f));


	return true;
}


bool Object::init(std::string _vertPath, std::string _fragPath,std::string _geomPath, Camera* _camera, bool genVAO)
{
	vertexShader.loadShaderFromFile("Resources/Shaders/" + _vertPath, GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders/" + _fragPath, GL_FRAGMENT_SHADER);
	geometeryShader.loadShaderFromFile("Resources/Shaders/" + _geomPath, GL_GEOMETRY_SHADER);
	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded() || !geometeryShader.isLoaded())
	{
		return false;
	}
	program.createProgram();
	program.addShaderToProgram(vertexShader);
	program.addShaderToProgram(fragmentShader);
	program.addShaderToProgram(geometeryShader);

	if (!program.linkProgram())
	{
		return false;
	}
	if (genVAO)
	{
		glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
		glBindVertexArray(mainVAO);
	}

	GLfloat points[] = { 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f };			//passing in 1 point

	camera = _camera;
	shapesVBO.addUpload(points, 0);
	//colorsVBO.addUpload(glm::vec3(1.0f, 0.0f, 0.0f), 1);
	transform.scale = glm::vec3(1.0f);
	transform.position = glm::vec3(0.0f, 0.0f, 18.0f);
	modelMatrix = glm::translate(modelMatrix, transform.position);
	return true;
}

bool Object::init(shader _vertex, shader _frag, GLuint _VAO, VertexBufferObject _shapes, VertexBufferObject _colors, Camera* _cam, Terrain* _terrain)
{
	vertexShader = _vertex;
	fragmentShader = _frag;

	program.createProgram();
	program.addShaderToProgram(vertexShader);
	program.addShaderToProgram(fragmentShader);

	if (!program.linkProgram())
	{
		return false;
	}
	if (_VAO == 0)
		mainVAO = _VAO;
	else
	glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
	glBindVertexArray(mainVAO);

	shapesVBO = _shapes;
	colorsVBO = _colors;
	camera = _cam;
	transform.scale = glm::vec3(1.0f);
	terrain = _terrain;
	_numVertices = 36;
	return true;

}

void Object::setVertexAttributesPointers(int numVertices)
{
	uint64_t offset = 0;
	 
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(offset));

		offset += sizeof(glm::vec3) * numVertices;
	}

	
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(offset));

		offset += sizeof(glm::vec2) * numVertices;
	}

	
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(offset));

		offset += sizeof(glm::vec3) * numVertices;
	}
}
Object::Object()
{
	modelMatrix = glm::mat4();
	transform.scale = glm::vec3(1.0f);
	modelMatrix = glm::scale(modelMatrix, transform.scale);
	transform.scale = glm::vec3(0.01f);
}

void Object::quadLOD()
{
	float distance = glm::length(transform.position - camera->getPosition());
	if (distance > 200.0f)
	{
 		program["gOuter"] = 2.0f;
		program["gInner"] = 1.0f;
	}

	else if(distance >150.0f)
	{
		program["gOuter"] = 5.0f;
		program["gInner"] = 3.0f;
	}

	else if(distance > 75.0f)
	{
		program["gOuter"] = 12.0f;
		program["gInner"] = 6.0f;
	}

	else
	{
		program["gOuter"] = 16.0f;
		program["gInner"] = 8.0f;
	}



}

void Object::Update(float _dT)
{
	modelMatrix[3][0] = transform.position.x;
	modelMatrix[3][1] = transform.position.y;
	modelMatrix[3][2] = transform.position.z;

	if (terrain != nullptr)
	{
		//std::cout << terrain->getRenderedHeightAtPosition(glm::vec3(256.0f, 0.0f, 128.0f), transform.position) << "\t" << transform.position.y << '\n';

		if (transform.position.y < terrain->getRenderedHeightAtPosition(glm::vec3(128.0f), transform.position))
		{
			transform.position.y = terrain->getRenderedHeightAtPosition(glm::vec3(128.0f), transform.position);
			//velocity.y = 0.0f;
		}
		else
		{
			velocity.y = -0.01f;
		}

	}
	transform.position += velocity * _dT;
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, transform.scale);
}

void Object::Render()
{
	if (alive)
	{
		program.useProgram();
		glBindVertexArray(mainVAO);
		program["PVM"] = camera->Project(modelMatrix);
		program["gSampler"] = 0;
		texture.bind();
		glDrawArrays(GL_TRIANGLES, 0, _numVertices);
		glBindVertexArray(0);
		program.unUseProgram();
	}
}

void Object::RenderQuad()
{
	program.useProgram();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 model = glm::mat4();

	quadLOD();
	program["PVM"] = camera->Project(glm::scale(modelMatrix, glm::vec3(1.0f)));

	glBindVertexArray(mainVAO);
	glDrawArrays(GL_PATCHES, 0, 6);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	program.unUseProgram();
}

void Object::RenderGeom()
{
	program.useProgram();
	glm::mat4 model = glm::mat4();
	program["PVM"] =	camera->Project(glm::scale(model, glm::vec3(5.0f)));
	//program["gSampler"] = 0;
	//texture.bind();
	glBindVertexArray(mainVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	program.unUseProgram();
}

void Object::setTexture(std::string _texture)
{
	texture.loadTexture2D(_texture, true);	
}


template <typename T>
void Object::setVertData(const T& _data)
{
	shapesVBO.addUpload(_data);
}


template <typename T>
void Object::setFragData(const T& _data)
{
	colorsVBO.addUpload(_data, 1);
}

template <typename T>
void Object::setGeomData(const T& _data)
{
	geomVBO.addUpload(_data);
	geomVBO.addUpload(_data);
}