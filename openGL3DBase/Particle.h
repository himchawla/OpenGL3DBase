#pragma once
#include <glew.h>
#include <glm.hpp>
#include <iostream>
#include <vector>

const int TOP = 0;
const int LEFT = 1;
const int BOTTOM = 2;
const int RIGHT = 3;

#include "Headers.h"
#include <gtc/matrix_transform.hpp>
class Particle
{
public:

	struct adj
	{
		adj(Particle* _point, int _position)
		{
			point = _point;
			position = _position;
		}
		Particle* point;
		int position;
	};
	struct vertex
	{
		vertex(glm::vec3 _v)
		{
			x = _v.x;
			y = _v.y;
			z = _v.z;
		}
		vertex(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}
		GLfloat x, y, z;
	};

	std::vector<std::vector<Particle>>* m_vertices;

	int m_z;

	int m_x;

	GLuint EBO;

	void init(std::vector <std::vector<Particle>> *_vertices, int x, int z)
	{
		m_x = x;
		m_z = z;
		m_vertices = _vertices;
		if(z > 0)
		{
			top = &(*_vertices)[x][z - 1];
			adjacent.push_back(adj(top, TOP));
		}
		if(x > 0)
		{
			left = &(*_vertices)[x - 1][z];
			adjacent.push_back(adj(left, LEFT));
		}
		if(x < 19)
		{
			right = &(*_vertices)[x + 1][z];
			adjacent.push_back(adj(right, RIGHT));
		}

		if (z < 19)
		{
			bottom = &(*_vertices)[x][z + 1];
			adjacent.push_back(adj(bottom, BOTTOM));
		}
		

		std::vector<vertex> verts;

		if (top != nullptr)
			verts.push_back(vertex(top->m_position));
		if (bottom != nullptr)
			verts.push_back(vertex(bottom->m_position));
			verts.push_back(m_position);
		if (left != nullptr)
			verts.push_back(vertex(left->m_position));
		if (right != nullptr)
			verts.push_back(vertex(right->m_position));

		if (top && bottom && right && left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(0);

			
		}
		if (top && bottom && right && left)
		{
			
		}
	

		
	}
	
	void RenderSolid()
	{
		glBindVertexArray(VAO);
		if (top && bottom && right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
			};

			GLuint i[] =
			{
				1, 4, 2, 5
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		if (!top && bottom && right && left)
		{
			GLfloat v[] = {
				//m_position.x,m_position.y, m_position.z,
				bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
				m_position.x,m_position.y, m_position.z,
				left->m_position.x,left->m_position.y, left->m_position.z,
				right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		if (top && !bottom && right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					//m_position.x,m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		if (top && bottom && !right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					//m_position.x,m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		if (top && bottom && right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					//m_position.x,m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		if (top && bottom && !right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if (!top && !bottom && right && left)
		{
			GLfloat v[] = {
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if (!top && bottom && !right && left)
		{
			GLfloat v[] = {
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		if (!top && bottom && right && !left)
		{
			GLfloat v[] = {
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		if (top && !bottom && !right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		if (top && !bottom && right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x,m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


		if (top && !bottom && !right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && bottom && !right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					/*top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && !bottom && right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
					/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && !bottom && !right && left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					//		top->m_position.x,top->m_position.y, top->m_position.z,
							/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
							m_position.x, m_position.y, m_position.z,
							right->m_position.x,right->m_position.y, right->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
	}

	void
	Render()
	{
		glBindVertexArray(VAO);
		if (top && bottom && right && left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			

			glDisable(GL_CULL_FACE);


			
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 6);
		}

		if (!top && bottom && right && left)
		{
			GLfloat v[] = {
					//m_position.x,m_position.y, m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x,m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 6);
		}
		if (top && !bottom && right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x,m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 6);
		}
		if (top && bottom && !right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x,m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}
		if (top && bottom && right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x,m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}
		if (top && bottom && !right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if (!top && !bottom && right && left)
		{
			GLfloat v[] = {
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,

			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if (!top && bottom && !right && left)
		{
			GLfloat v[] = {
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}

		if (!top && bottom && right && !left)
		{
			GLfloat v[] = {
					bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}
		
		if (top && !bottom && !right && left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}

		if (top && !bottom && right && !left)
		{
			GLfloat v[] = {
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x,m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
			};

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 4);
		}


		if (top && !bottom && !right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,*/
			};
			
			glDisable(GL_CULL_FACE);


		

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && bottom && !right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
			        bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					/*top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && !bottom && right && !left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,
					/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					top->m_position.x,top->m_position.y, top->m_position.z,
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		if (!top && !bottom && !right && left)
		{
			GLfloat v[] = {
					m_position.x, m_position.y, m_position.z,
					left->m_position.x,left->m_position.y, left->m_position.z,
			//		top->m_position.x,top->m_position.y, top->m_position.z,
					/*bottom->m_position.x,bottom->m_position.y, bottom->m_position.z,
					m_position.x, m_position.y, m_position.z,
					right->m_position.x,right->m_position.y, right->m_position.z,*/
			};

			glDisable(GL_CULL_FACE);




			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);


			glDrawArrays(GL_LINES, 0, 2);
		}
		


	}

	
	Particle *top = nullptr,
			 *bottom = nullptr,
			 *left = nullptr,
			 *right = nullptr;

	std::vector<adj> adjacent;
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
		
			if (left)
			{

				if (glm::length(left->m_position - m_position) < 0.055)
				{
					position = position + velocity * dt;
					velocity = velocity + (force / mass) * dt;
					t = t + dt;
					if (m_accelaration.z != 0.0f)
						m_position.z += position * 20.0f;
					if (position > 0.0f)
					{
						std::cout << "";
					}
				}
				else return;

			}
			
			

			else if (bottom)
			{

				if (glm::length(bottom->m_position - m_position) < 0.055)
				{
					position = position + velocity * dt;
					velocity = velocity + (force / mass) * dt;
					t = t + dt;
					if (m_accelaration.z != 0.0f)
						m_position.z += position * 20.0f;
					if (position > 0.0f)
					{
						std::cout << "";
					}
				}

			}

			else if (top)
			{


				if (abs(top->m_position.z - m_position.z) < 0.06)
				{
					/*position = position + velocity * dt;
					velocity = velocity + (force / mass) * dt;
					t = t + dt;
					if (m_accelaration.z != 0.0f)
						m_position.z += position * 20.0f;
					if (position > 0.0f)
					{
						std::cout << "";
					}*/
				}

			
		}

		if (t <= 20)
		{
			
		}

		else
		{
			
		}
		
	}

	void LateUpdate()
	{
		m_pPosition = m_position;
	}


private:
	GLuint  VAO;
	GLuint VBO;
	
};

