#pragma once
#include <glew.h>
#include <random>
#include<glm.hpp>

#include "Camera.h"
#include "shaderProgram.h"


class Particle
{
private:

	glm::vec3 m_position;
	glm::vec3 m_origin;
	glm::vec3 m_velocity;
	float m_elapsedTime;

	int m_id;

public:
	Particle(int _id , glm::vec3 _origin)
	{
		m_id = _id;
		m_origin = _origin;
	}
	float randomFloat();

	void resetToInitialValues();

	void Update(float _dT);
	glm::vec3 getPosition() { return m_position; }
};


class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 origin, Camera* _camera, std::string texFileName);
	ParticleSystem(glm::vec3 origin, Camera* _camera, std::string texFileName, bool gpu);
	~ParticleSystem();

	void render(float dt);
	void renderGpu(float dt);
	std::vector<Particle> particles;
	std::vector<glm::vec3> vPosition;
	
	std::vector<glm::vec4> initPos;
	std::vector<glm::vec4> initVel;
private:
	float randomFloat();
	shader vertex, fragment, geometery;
	shader compute;
	Camera* camera;
	GLuint vao, vbo, texture;

	GLuint initPosVBO;
	GLuint posVbo, velVbo, initVelVbo;
	ShaderProgram m_program, m_computeProgram;
	float nParticles;
};

