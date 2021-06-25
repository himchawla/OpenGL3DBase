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
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* vPosition.size(), &vPosition[0],
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


ParticleSystem::ParticleSystem(glm::vec3 origin, Camera* _camera, std::string texFileName, bool gpu)
{
	camera = _camera;

	nParticles = 128 * 2000;
	FOR(i, nParticles)
	{
		initPos.push_back(glm::vec4(randomFloat() * 200.0f - 100.0f, 200.0f, randomFloat() * 200.0f - 100.0f, randomFloat() + 0.125));
		initVel.push_back(glm::vec4(
			0,
			1.0 + 0.25f * randomFloat() - 0.125f,
			0,
			randomFloat() * 10 + 1.0f));
		/*initVel.push_back(glm::vec4(
			0.25 * cos(i * .0167 * 0.5) + 0.25f * randomFloat() - 0.125f,
			2.0f + 0.25f * randomFloat() - 0.125f,
			0.25 * sin(i * .0167 * 0.5f) + 0.25f * randomFloat() - 0.125f,
			1.0f));*/
	}

	m_program.createProgram();

	vertex.loadShaderFromFile("Resources/Shaders/particleGPU.vert", GL_VERTEX_SHADER);
	fragment.loadShaderFromFile("Resources/Shaders/particleGPU.frag", GL_FRAGMENT_SHADER);
	geometery.loadShaderFromFile("Resources/Shaders/Geometery.geom", GL_GEOMETRY_SHADER);

	m_program.addShaderToProgram(vertex);
	m_program.addShaderToProgram(geometery);
	m_program.addShaderToProgram(fragment);

	if (!m_program.linkProgram())
	{
		throw("Particle program can't be linked");
	}

	m_computeProgram.createProgram();
	compute.loadShaderFromFile("Resources/Shaders/particle.comp", GL_COMPUTE_SHADER);
	m_computeProgram.addShaderToProgram(compute);
	if (!m_computeProgram.linkProgram())
	{
		throw("Particle compute program can't be linked");
	}


	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * initPos.size(), &initPos[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posVbo);

	
	glGenBuffers(1, &velVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4)* initVel.size(), &initVel[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velVbo);

	glGenBuffers(1, &initVelVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initVelVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4)* initVel.size(), &initVel[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, initVelVbo);

	glGenBuffers(1, &initPosVBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initPosVBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4)* initPos.size(), &initPos[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, initPosVBO);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, (GLvoid*)0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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

void ParticleSystem::renderGpu(float dt)
{
	m_computeProgram.useProgram();
	glDispatchCompute(nParticles / 128, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glm::vec3 vQuad1, vQuad2;
	glm::vec3 camFront = camera->getCameraFront();
	camFront = glm::normalize(camFront);
	vQuad1 = glm::cross(camFront, camera->getCameraUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(camFront, vQuad1);
	vQuad2 = glm::normalize(vQuad2);
	
	m_program.useProgram();
	m_program["vp"] = camera->getProjMatrix() * camera->getViewMatrix();
	m_program["vQuad1"] = vQuad1;
	m_program["vQuad2"] = vQuad2;
	
	//glActiveTexture(GL_TEXTURE0);
	//m_program["Texture"] = 0;
	//glBindTexture(GL_TEXTURE_2D, texture);

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);*/

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);
	
	glDrawArrays(GL_POINTS, 0, nParticles);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

	
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);




}
float ParticleSystem::randomFloat()
{
	float r = (float)rand() / (double)RAND_MAX;
	return r;
}
