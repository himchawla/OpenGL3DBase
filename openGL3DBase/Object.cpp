#include "Object.h"

#include "Camera.h"

bool Object::init(std::string _vertPath, std::string _fragPath, Camera* _camera)
{
	vertexShader.loadShaderFromFile("Resources/Shaders/" + _vertPath, GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("Resources/Shaders" + _fragPath, GL_FRAGMENT_SHADER);

	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
	{
		return false;
	}

	camera = _camera;
	glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
	glBindVertexArray(mainVAO);

	return true;
}

bool Object::init(shader _vertex, shader _frag, GLuint _VAO, VertexBufferObject _shapes, VertexBufferObject _colors, Camera* _cam)
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
	return true;
}

Object::Object()
{
	modelMatrix = glm::mat4();
}

void Object::Update()
{
	modelMatrix[3][0] = transform.position.x;
	modelMatrix[3][1] = transform.position.y;
	modelMatrix[3][2] = transform.position.z;
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, transform.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, transform.scale);
}

void Object::Render()
{
	program.useProgram();
	glBindVertexArray(mainVAO);
	program["PVM"] = camera->Project(modelMatrix);
	program["gSampler"] = 0;
	texture.bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
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
	colorsVBO.addUpload(_data);
}

