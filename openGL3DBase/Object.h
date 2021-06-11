#pragma once
#include "shaderProgram.h"
#include "VertexBufferObject.h"
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include "Camera.h"
#include "sampler.h"
#include "Texture.h"

class Terrain;

class Object
{
public:
	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	}transform;

	bool alive = true;
	bool isGrounded = false;

	glm::vec3 velocity;

	Terrain* terrain;
	GLuint vbo;
	GLuint vao;

	void move(const std::function<bool(int)>& keyInputFunc);
	bool init(std::string _vertPath, std::string _fragPath, Camera* _camera, bool genVAO = true);
	bool initQuad(std::string _vertPath, std::string _fragPath, Camera* _camera, bool genVAO);
	bool init(std::string _vertPath, std::string _fragPath, std::string _geomPath, Camera* _camera, bool genVAO);
	bool init(shader _vertex, shader _frag, GLuint _VAO, VertexBufferObject _shapes, VertexBufferObject _colors,
	          Camera* _cam, Terrain* _terrain);
	void setVertexAttributesPointers(int numVertices);

	template<typename T>
	void setVertData(const T& _data);

	template<typename T>
	void setFragData(const T& _data);
	template <class T>
	void setGeomData(const T& _data);

	Object();
	void quadLOD();

	void Update(float _dT);
	void Render();
	void RenderQuad();
	void RenderGeom();

	void setTexture(std::string _texture);

	VertexBufferObject& GetVBO()
	{
		return shapesVBO;
	}

	void setVerts(int numVertices)
	{
		_numVertices = numVertices;
	}
protected:
	glm::mat4 modelMatrix;
	Camera* camera;

	GLuint mainVAO;

	shader tesShader;
	shader tcsShader;
	shader geometeryShader;
	shader vertexShader, fragmentShader;
	ShaderProgram program;
	VertexBufferObject shapesVBO;
	VertexBufferObject indexVBO;
	VertexBufferObject colorsVBO;
	VertexBufferObject geomVBO;

	Texture texture;

private:
	int _numVertices;
};
