#include "Particle.h"

#include "Headers.h"

float Particle::randomFloat()
{
	float r = (float)rand() / (double)RAND_MAX;
	return r;
}

void Particle::resetToInitialValues()
{
	this->m_position = this->m_origin;
	this->m_velocity = glm::vec3(
		0.25 * cos(this->m_id * .0167) + 0.25f * randomFloat() - 0.125f,
		1.5f + 0.25f * randomFloat() - 0.125f,
		0.25 * sin(this->m_id * .0167) + 0.25f * randomFloat() - 0.125f);
	this->m_elapsedTime = randomFloat() + 0.125;
}

void Particle::Update(float _dT)
{
	this->m_velocity.y += -0.2 * _dT;
	this->m_position += m_velocity;
	this->m_elapsedTime -= _dT;
	if (this->m_elapsedTime <= 0.0f)
	{
		resetToInitialValues();
	}
}

ParticleSystem::ParticleSystem(glm::vec3 origin, Camera* _camera, std::string texFileName)
{
	camera = _camera;
	
	nParticles = 4000;
	FOR(i, nParticles)
	{
		vPosition.push_back(glm::vec3(0.0f));
		Particle P(i, origin);
		particles.push_back(P);
	}

	m_program.createProgram();

	vertex.loadShaderFromFile("Resources/Shaders/particle.vert", GL_VERTEX_SHADER);
	fragment.loadShaderFromFile("Resources/Shaders/particle.frag", GL_FRAGMENT_SHADER);
	geometery.loadShaderFromFile("Resources/Shaders/particle.geom", GL_GEOMETRY_SHADER);
	
	m_program.addShaderToProgram(vertex);
	m_program.addShaderToProgram(geometery);
	m_program.addShaderToProgram(fragment);

	if(!m_program.linkProgram())
	{
		throw("Particle program can't be linked");
	}
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenVertexArrays(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* vPosition.size(), &vPosition[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::render(float dt)
{
	for (int i = 0; i < nParticles; i++) {
		particles[i].Update(.0167);
		vPosition[i] = particles[i].getPosition();
	}

	glm::vec3 vQuad1, vQuad2;
	glm::vec3 camFront = camera->getCameraFront();
	camFront = glm::normalize(camFront);
	vQuad1 = glm::cross(camFront, camera->getCameraUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(camFront, vQuad1);
	vQuad2 = glm::normalize(vQuad2);

	m_program.useProgram();
	m_program["vQuad1"] = vQuad1;
	m_program["vQuad2"] = vQuad2;
	m_program["vp"] = camera->getProjMatrix() *  camera->getViewMatrix();

	glActiveTexture(GL_TEXTURE0);
	m_program["Texture"] = 0;
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vPosition.size(), &vPosition[0],
		GL_STATIC_DRAW);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, nParticles);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	
	
	
}
