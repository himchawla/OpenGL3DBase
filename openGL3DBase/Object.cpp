#include "Object.h"

#include "Camera.h"
#include "Terrain.h"

void Object::move(const std::function<bool(int)>& keyInputFunc)
{
	if(keyInputFunc('w'))
	{
		transform.position.z += 0.01f;
	}
	if (keyInputFunc('s'))
	{
		transform.position.z -= 0.01f;
	}
	if (keyInputFunc('a'))
	{
		transform.position.x += 0.01f;
	}
	if (keyInputFunc('d'))
	{
		transform.position.x -= 0.01f;
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
	transform.position = (glm::vec3(6.0f, 1.0f, 0.0f));

	translate(modelMatrix, transform.position);

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
	program.addShaderToProgram(geometeryShader);
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
	shapesVBO.addUpload(glm::vec3(0.5f,0.5f,0.5f), 0);
	colorsVBO.addUpload(glm::vec3(1.0f, 0.0f, 0.0f), 1);
	transform.scale = glm::vec3(1.0f);
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

void Object::Update(float _dT)
{
	modelMatrix[3][0] = transform.position.x;
	modelMatrix[3][1] = transform.position.y;
	modelMatrix[3][2] = transform.position.z;

	if (terrain != nullptr)
	{
		std::cout << terrain->getRenderedHeightAtPosition(glm::vec3(512.0f, 0.0f, 512.0f), transform.position) << "\t" << transform.position.y << '\n';

		if (transform.position.y < terrain->getRenderedHeightAtPosition(glm::vec3(512.0f), transform.position))
		{
			transform.position.y = terrain->getRenderedHeightAtPosition(glm::vec3(512.0f), transform.position);
			velocity.y = 0.0f;
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
		glDrawArrays(GL_TRIANGLES, 0, 36);
		program.unUseProgram();
	}
}

void Object::RenderGeom()
{
	program.useProgram();
	glBindVertexArray(mainVAO);
	
	program["PVM"] = (modelMatrix);
	program["gSampler"] = 0;
	//texture.bind();
	glDrawArrays(GL_POINT, 0, 1);
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