#pragma once
#include <glm.hpp>
#include <iostream>

class Particle
{
public:
	glm::vec3 m_position;
	glm::vec3 m_accelaration = glm::vec3(0.0f, 0.0f,8.0f);
	glm::vec3 m_velocity;
	glm::vec3 m_nPosition;
	glm::vec3 m_pPosition;
	float t = 0.0f;
	float dt = 1.0f/ 600000.0f; //Seconds
	float velocity = 0.0f;
	float position = 0.0f;
	float force = 0.5f;
	float mass = 1.0f;
	void Integrate(float _dT)
	{
		

		if (t <= 20)
		{
			position = position + velocity * dt;
			velocity = velocity + (force / mass) * dt;
			t = t + dt;
			if(m_accelaration.z != 0.0f)
				m_position.z += position * 20.0f;
			if(position > 0.0f)
			{
 				std::cout << "";
			}
		}

		else
		{
			
		}
		
	}

	void LateUpdate()
	{
		m_pPosition = m_position;
	}
};

