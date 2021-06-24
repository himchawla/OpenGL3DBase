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

	bool m_releaseAll;
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

	void Color()
	{
		
	}

	bool m_isFixed = false;
	
	std::vector<std::vector<Particle>>* m_vertices;

	int m_z;

	int m_x;

	GLuint EBO;
	bool isTop = false;
	void init(std::vector <std::vector<Particle>> *_vertices, int x, int z, float tolerance = 0.06, bool sec = false)
	{
		m_tolerance = tolerance + tolerance * 0.14f;
		m_accelaration = glm::vec3(0.0f);
		m_x = x;
		m_z = z;
		m_vertices = _vertices;
		m_pPosition = m_position;
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
		if(x < _vertices->size() - 1)
		{
			right = &(*_vertices)[x + 1][z];
			adjacent.push_back(adj(right, RIGHT));
		}

		if (z < _vertices[0].size() -1)
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

			if(!sec)
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			if (!sec)
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
			//glDrawArrays(GL_TRIANGLES, 0, 3);
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

	void Render()
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
	glm::vec3 m_accelaration = glm::vec3(0.0f, 0.0f,0.0f);
	glm::vec3 m_velocity;
	glm::vec3 m_nPosition;
	glm::vec3 m_pPosition;
	float t = 0.0f;
	float dt = 1.0f/ 600.0f; //Seconds
	float velocity = 0.0f;
	float position = 0.0f;
	float force = 0.5f;
	float mass = 1.0f;
	bool recentlyReleased;
	float releaseTimer;

	bool Left(float _tolerance = 0.06, bool _deafault = true)
	{
		if (!left) return _deafault;
		if(glm::length(left->m_position - m_position) < _tolerance)
		{
			return true;
		}
		return false;
	}


	bool Right(float _tolerance = 0.06, bool _deafault = true)
	{
		if (!right) return _deafault;
		if (glm::length(right->m_position - m_position) < _tolerance)
		{
			return true;
		}
		return false;
	}

	bool Bottom(float _tolerance = 0.06, bool _deafault = true)
	{
		if (!bottom) return _deafault;
		if (glm::length(bottom->m_position - m_position) < _tolerance)
		{
			return true;
		}
		return false;
	}

	bool Top(float _tolerance = 0.06, bool _deafault = true)
	{
		if (!top) return _deafault;
		if (glm::length(top->m_position - m_position) < _tolerance)
		{
			return true;
		}
		return false;
	}

	void preCalcs()
	{
		if (m_isFixed) 
			return;
		
		
		
			m_accelaration.z = 80.0f;
			if (m_position.z >= 0.8f)
			{
				
			}
		
		
	}

	void updateBS()
	{
		if (top)
			top->bottom = this;
		if (right)
			right->left = this;
		if (left)
			left->right = this;
		if (bottom)
			bottom->top = this;
	}

	void intoConstraint()
	{
		float distance = m_tolerance;
		if (m_isFixed) return;
		if (left)
		{
			glm::vec3 v = left->m_position - m_position;
			
			v *= ((1.0f - distance / glm::length(v)) * 0.8f);

			if (!left->m_isFixed)
			{
				left->m_position -= v;
				m_position += (v);
			}
			else m_position += 2.0f * v;
		}
		
		if (bottom)
		{
			glm::vec3 v = bottom->m_position - m_position;
			v *= ((1.0f - distance / glm::length(v)) * 0.8f);
			if (!bottom->m_isFixed)
			{
				bottom->m_position -= v;
				m_position += (v);
			}
			else m_position += 2.0f * v;
		}
		//updateBS();
	}

	bool m_released;

	float m_tolerance;

	void Integrate(float _dT)
	{
		//if (Left(0.06) && Top(0.06) && Bottom(0.06))
		//{
		//	m_nPosition = 2.0f * m_position - m_pPosition + m_accelaration * _dT * _dT;
		//	//m_nPosition = m_position;
		//	m_pPosition = m_position;
		//	m_position = m_nPosition;
		//	//updateBS();
		//}

		if ((m_position.z < 0.8f) && Left(m_tolerance) && Top(m_tolerance) && Bottom(m_tolerance) && Right(m_tolerance))
		{
			m_nPosition = 2.0f * m_position - m_pPosition + m_accelaration * _dT * _dT;
			//m_nPosition = m_position;
			m_pPosition = m_position;
			m_position = m_nPosition;
			/*if (glm::length(m_accelaration) > 3.0f)
				m_releaseAll = true;*/
			//updateBS();

		}
		
		if (t <= 20)
		{
			
		}

		else
		{
			
		}
		
	}

	void postCal()
	{
		
		if (m_position.z > 0.8f)
		{
			m_accelaration = glm::vec3(0);
			m_position.z = 0.8f;

			intoConstraint();
		}
		else
		{
			if (glm::length(m_accelaration) > 605.0f)
			{
				m_releaseAll = true;
			}
			
			
			if (m_accelaration.x < -0.01f)
			{
				//std::cout << m_accelaration.x << '\n';
				m_accelaration.x -= m_accelaration.x * 0.167;

			}
			else if(m_accelaration.x < 0.0f)
			{
				m_accelaration.x = 0.0f;
			}

			

			if (m_accelaration.y < -0.01f)
			{
				m_accelaration.y -= m_accelaration.y * 0.167;
			}
			else if (m_accelaration.y < 0.0f)
			{
				m_accelaration.y = 0.0f;
			}


			
			if (m_accelaration.x > 0.01f)
			{
				m_accelaration.x -= m_accelaration.x * 0.167;
			}
			else if (m_accelaration.x > 0.0f)
			{
				m_accelaration.x = 0.0f;
			}

			if (m_accelaration.y > 0.01f)
			{
				m_accelaration.y -= m_accelaration.y * 0.167;
			}
			else if (m_accelaration.y > 0.0f)
			{
				m_accelaration.y = 0.0f;
			}

			m_accelaration.z = 0.0f;

			
			
			intoConstraint();
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

