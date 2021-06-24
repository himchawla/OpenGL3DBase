#include "Terrain.h"

#include <stb_image.h>
#include <detail/_vectorize.hpp>
#include <detail/_vectorize.hpp>
#include <detail/_vectorize.hpp>
#include <detail/_vectorize.hpp>

#include "Headers.h"
//#include <ft2build.h>
//#include <freetype.h>

Cloth::Cloth()
		:Object()
{
	m_rows = 20;
	m_columns = 20;
}

void Cloth::init(Camera* _cam)
{
    Object::init("vertex.vert", "fragment.frag", _cam, 0);
    createCloth();
    modelMatrix = glm::scale(modelMatrix, glm::vec3(128.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    flag = false;
	
}


void Cloth::createCloth()
{
    if (m_isInitialized) {
        shapesVBO.deleteVBO();
    }

	m_heightData.resize(m_rows);
	FOR(i, m_rows) m_heightData[i].resize(m_columns);
    m_rows = static_cast<int>(m_heightData.size());
    m_columns = static_cast<int>(m_heightData[0].size());
    m_numVertices = m_rows * m_columns;

    // First, prepare VAO and VBO for vertex data
    glGenVertexArrays(1, &mainVAO);
    glBindVertexArray(mainVAO);
    shapesVBO.createVBO(m_numVertices * getVertexByteSize()); // Preallocate memory
    shapesVBO.bindVBO();
    setUpVertices();
    for (auto i = 0; i < m_rows; i++)
    {
        shapesVBO.addRawData(m_vertices[i].data(), m_columns * sizeof(glm::vec3));
    }
    setUpTextureCoordinates();
    //setUpNormals();

    // Send data to GPU, they're ready now
    shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);
    shapesVBO.unmapBuffer();

    setVertexAttributesPointers(m_numVertices);

    // Vertex data are in, set up the index buffer
    setUpIndexBuffer();

    // Clear the data, we won't need it anymore
    //m_vertices.clear();
    m_textureCoordinates.clear();
    m_normals.clear();
    m_vertices[1][1].m_isFixed  = true;
    m_vertices[1][4].m_isFixed  = true;
    m_vertices[1][9].m_isFixed =  true;
    m_vertices[1][14].m_isFixed = true;
    m_vertices[1][18].m_isFixed = true;

		 /*m_vertices[1][1].m_position.z -= 0.02f;
		 m_vertices[1][4].m_position.z -= 0.02f;
		 m_vertices[1][9].m_position.z -= 0.02f;
		m_vertices[1][14].m_position.z -= 0.02f;
		m_vertices[1][18].m_position.z -= 0.02f;*/

    // If get here, we have succeeded with generating heightmap
    m_isInitialized = true;
}

void Cloth::Render()
{
    if (!m_isInitialized) {
        return;
    }
  
    //m_vertices[0][6].m_position.y += 0.0002f;
	//m_vertices[0][7].y = 1.0f;*/
   // shapesVBO.createVBO(m_numVertices * getVertexByteSize()); // Preallocate memory
    
    shapesVBO.bindVBO();
    //setUpVertices();
    for (auto i = 0; i < m_rows; i++)
    {
        std::vector<glm::vec3>temp;
        temp.resize(m_columns);
        FOR(j, m_columns) temp[j] = m_vertices[i][j].m_position;
        shapesVBO.addRawData(temp.data(), m_columns * sizeof(glm::vec3));
    }
    setUpTextureCoordinates();
    //setUpNormals();

    // Send data to GPU, they're ready now
    shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);
    shapesVBO.unmapBuffer();
	program.useProgram();
    glCullFace(GL_BACK);
	//glBindVertexArray(mainVAO);
   /* glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(m_numVertices);*/
    transform.position.x = 0.0f;
    program["PVM"] = camera->Project(modelMatrix);
    //program["PVM"] = camera->Project(glm::scale(modelMatrix, glm::vec3(128.0f)));
   // program["gSampler"] = 0;
   /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLE_STRIP, m_numIndices, GL_UNSIGNED_INT, 0);
    glDisable(GL_PRIMITIVE_RESTART);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	SFOR(i, 2, m_rows - 2, 1)
	{
		SFOR(j, 1, m_columns - 2, 1)
		{
			if((i+j) %2)
				program["color"] = glm::vec4(1.0, 0.0, 0.0, 1.0);
			else
				program["color"] = glm::vec4(0.0, 1.0, 0.0, 1.0);
			if (m_vertices[i][j].m_releaseAll)
				m_releaseAll = true;
			//texture.bind();
			if (m_wireFrame)
				m_vertices[i][j].Render();
			else
				m_vertices[i][j].RenderSolid();

			
		}
	}

	if (m_releaseAll)
	{
		FOR(i, m_columns)
			m_vertices[1][i].m_isFixed = false;
	}

   // m_vertices[1][1].RenderSolid();
   // m_vertices[1][2].RenderSolid();
   
	
}


void Cloth::move(const std::function<bool(int)>& keyInputFunc)
{
	if(keyInputFunc('p'))
	{
		m_wireFrame = !m_wireFrame;
	}
	if(keyInputFunc('y'))
	{
		std::cout << "Rows: ";
		std::cin >> m_columns;
		std::cout << "\nColumns: ";
		std::cin >> m_rows;

		if (m_rows != m_columns)
			m_rows = m_columns;
		

		setUpVertices(false);
		FOR(i, m_rows) FOR(j, m_columns) {
			m_vertices[i][j].m_released = false;
			m_vertices[i][j].m_accelaration = glm::vec3(0.0);
			m_vertices[i][j].m_releaseAll = false;
		}
		m_releaseAll = false;

		std::cout << m_columns / m_hooks;
		FOR(i, 20)	m_vertices[1][i].m_isFixed = false;
		SFOR(j, 0, m_columns - m_columns / m_hooks, m_columns / m_hooks)
			m_vertices[1][j].m_isFixed = true;
		m_vertices[1][m_columns - 1].m_isFixed = true;
		m_tolerance = glm::length(m_vertices[0][0].m_position - m_vertices[0][1].m_position);
		
	}
	if(keyInputFunc('n'))
	{
		if(cube->transform.scale.x < 1000.0f)
		{
           // cube->transform.scale = cube->transform.scale + glm::vec3(1.0f);
		}
	}
	if(keyInputFunc('c'))
	{
        Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
        if (temp != nullptr)
        {
            if (temp->left != nullptr) {
                temp->left->right = nullptr;
                temp->left = nullptr;
				temp->updateBS();
				//temp = nullptr;
            }
            if(temp->top != nullptr)
            {

                temp->top->bottom = nullptr;
                temp->top = nullptr;
				temp->updateBS();
            	temp = nullptr;

            }
                
            
        }
       
	}
    if (keyInputFunc('v'))
    {
        Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.z += m_fanSpeed * 0.5 * 0.0167f;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.z += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.z += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.z += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.z += m_fanSpeed * 0.5 * 0.0167;
			}
		}

    }

	if (keyInputFunc('s'))
	{
		Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167f;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
		}
	}

	if (keyInputFunc('b'))
	{
		Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.z -= m_fanSpeed * 0.5 * 0.0167f;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.z -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.z -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.z -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.z -= m_fanSpeed * 0.5 * 0.0167;
			}
		}

	}
	if (keyInputFunc('w'))
	{
		Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.y += m_fanSpeed * 0.8f * 0.0167;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.y += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.y += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.y += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.y += m_fanSpeed * 0.5 * 0.0167;
			}
		}

	}
	if (keyInputFunc('d'))
	{
		Particle* temp = renderedPosition(cube->transform.position.x, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.x += m_fanSpeed * 0.8f * 0.0167;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.x += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.x += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.x += m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.x += m_fanSpeed * 0.5 * 0.0167;
			}
		}

	}
	if (keyInputFunc('a'))
	{
		Particle* temp = renderedPosition(cube->transform.position.y, cube->transform.position.z);
		if (temp != nullptr)
		{
			temp->m_accelaration.y -= m_fanSpeed * 0.8f * 0.0167;
			if (temp->left != nullptr) {
				temp->left->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->top != nullptr)
			{
				temp->top->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->right != nullptr) {
				temp->right->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
			if (temp->bottom != nullptr)
			{
				temp->bottom->m_accelaration.y -= m_fanSpeed * 0.5 * 0.0167;
			}
		}

	}
	if(keyInputFunc('t'))
	{
		//setUpVertices(true);

		 FOR(i, m_columns)
		 {
			 m_vertices[1][i].m_isFixed = false;
		 }
		 

		// m_vertices[5][5].top = nullptr;
       // m_vertices[5][5].bottom = nullptr;
		/*m_vertices[5][5].bottom = nullptr;
        m_vertices[5][5].top = nullptr;*/

		FOR(i, m_rows) FOR(j, m_columns) m_vertices[i][j].m_released = true;
       
       //
       // m_vertices[2][2].left = nullptr;
	}

	if (keyInputFunc('r'))
	{

		setUpVertices(true);
		FOR(i, m_rows) FOR(j, m_columns) {
			m_vertices[i][j].m_released = false;
			m_vertices[i][j].m_accelaration = glm::vec3(0.0);
			m_vertices[i][j].m_releaseAll = false;
		}
		m_releaseAll = false;

		std::cout << m_columns / m_hooks;
		FOR(i, 20)	m_vertices[1][i].m_isFixed = false;
		SFOR(j, 0, m_columns - m_columns / m_hooks, m_columns / m_hooks)
		m_vertices[1][j].m_isFixed = true;
		m_vertices[1][m_columns - 1].m_isFixed = true;
		m_tolerance = glm::length(m_vertices[0][0].m_position - m_vertices[0][1].m_position);
	}

	if(keyInputFunc('h'))
	{
		std::cout << "Hooks :";
		std::cin >> m_hooks;
		m_hooks > m_columns ? m_hooks = m_columns:m_hooks = m_hooks;
		FOR(i, m_rows) FOR(j, m_columns) m_vertices[i][j].m_released = false;
		std::cout << m_columns / m_hooks;
		FOR(i, 20)	m_vertices[1][i].m_isFixed = false;

		SFOR(j, 0, m_columns - m_columns / m_hooks, m_columns / m_hooks)
			m_vertices[1][j].m_isFixed = true;
		m_vertices[1][m_columns - 1].m_isFixed = true;
		m_tolerance = glm::length(m_vertices[0][0].m_position - m_vertices[0][1].m_position);
		setUpVertices(true);
	}

	if(keyInputFunc('f'))
	{
        std::cin >> m_fanSpeed;
        if (m_fanSpeed < 5.0f) m_fanSpeed = 5.0f;
        else if (m_fanSpeed > 10.0f)  m_fanSpeed = 10.0f;
		m_fanSpeed *= 1000;
	}
	if(keyInputFunc('8'))
	{
		FOR(i, m_rows) FOR(j, m_columns)
			m_vertices[i][j].m_accelaration.y += m_fanSpeed * 0.0167f;
	}
	if(keyInputFunc('2'))
	{
		FOR(i, m_rows) FOR(j, m_columns)
			m_vertices[i][j].m_accelaration.y += -m_fanSpeed * 0.0167f;
	}

    if (keyInputFunc('4'))
    {
		FOR(i, m_rows) FOR(j, m_columns)
    		m_vertices[i][j].m_accelaration.x += -m_fanSpeed * 0.0167f;
    }

    if (keyInputFunc('6'))
    {
		FOR(i, m_rows) FOR(j, m_columns)
            m_vertices[i][j].m_accelaration.x += m_fanSpeed * 0.0167f;
    }
}


void Cloth::Update(float _dT)
{
	AFOR(i, m_vertices)
	{
		AFOR(j, i)
		{
			j.preCalcs();
            j.Integrate(0.0167f);
			j.postCal();
			if (m_releaseAll)
				j.m_released = true;
		}
	}

	

    AFOR(i, m_vertices)
    {
        AFOR(j, i)
        {
            //j.m_position += glm::vec3(0.0f, (flag)? 2.0f: -2.0f, 0.0f);
        }
    }

    flag = !flag;

    /* m_vertices[1][1].m_isFixed = true;
     m_vertices[1][4].m_isFixed = true;
     m_vertices[1][9].m_isFixed = true;
    m_vertices[1][14].m_isFixed = true;
    m_vertices[1][18].m_isFixed = true;*/

	
}

void Cloth::RenderPoints()
{
    if (!m_isInitialized) {
        return;
    }

    glBindVertexArray(mainVAO);

    // Render points only
    glDrawArrays(GL_LINES, 0, m_numVertices);
}




int Cloth::getRows() const
{
    return m_rows;
}

int Cloth::getColumns() const
{
    return m_columns;
}

float Cloth::getHeight(const int _row, const int _column) const
{
    if (_row < 0 || _row >= m_rows || _column < 0 || _column >= m_columns) {
        return 0.0f;
    }

    return m_heightData[_row][_column];
}

Particle* Cloth::renderedPosition(float x, float z)
{
    x /= 128.0f;
    z /= 128.0f;

    float d = 0.2;
    int xp = 0;
    int yp = 0;
	FOR(i, m_rows) FOR(j, m_columns)
	{
        glm::vec2 xz = glm::vec2(m_vertices[i][j].m_position.x, m_vertices[i][j].m_position.z);
        //std::cout << glm::length(xz - glm::vec2(x, z)) << "\t" << xz.x << "\t" << xz.y << "\t"<< x << "\t" << z << "\n";
		if(glm::length(xz - glm::vec2(x, z)) < d)
		{
            d = glm::length(xz - glm::vec2(x, z));
            xp = i; yp = j;
            //return &m_vertices[i][j];
		}
	}
    return &m_vertices[xp][yp];
}


void Cloth:: setUpVertices(bool sec)
{
	
	m_vertices.resize(m_rows);
    FOR(i, m_rows)   m_vertices[i].resize(m_columns);

	m_heightData.resize(m_rows);
	FOR(i, m_rows)	m_heightData[i].resize(m_columns);
    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
			//if ((i == 7 || i == 8) && j == 0) m_heightData[i][j] = 10.0f;
			//else
            m_heightData[i][j] = 0.0f;
            const auto factorRow = static_cast<float>(i) / static_cast<float>(m_rows - 1);
            const auto factorColumn = static_cast<float>(j) / static_cast<float>(m_columns - 1);
            const auto& fVertexHeight = m_heightData[i][j];
            m_vertices[i][j].m_position = glm::vec3(-0.5f + factorColumn, fVertexHeight, -0.5f + factorRow);
           
        }
    }
	

	
	{
		FOR(i, m_rows)
		{
			FOR(j, m_columns)
			{
				m_vertices[i][j].init(&m_vertices, i, j, m_tolerance, sec);
			}
		}

		SFOR(i, 1, m_columns - 1, 1)
		{
			m_vertices[1][i].left = nullptr;
			m_vertices[1][i].isTop = true;
			m_vertices[m_rows - 2][i].right = nullptr;
			//m_vertices[i][18].bottom = nullptr;
		}
	}
}

void Cloth::setUpTextureCoordinates()
{
    m_textureCoordinates = std::vector<std::vector<glm::vec2>>(m_rows, std::vector<glm::vec2>(m_columns));

    const auto textureStepU = 0.1f;
    const auto textureStepV = 0.1f;

    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++) {

        	if((i + j) % 2)
				m_textureCoordinates[i][j] = glm::vec2(0.0,0.0);

        	if((i + j) % 2 == 0)
				m_textureCoordinates[i][j] = glm::vec2(1.0,1.0);
        }
        shapesVBO.addRawData(m_textureCoordinates[i].data(), m_columns * sizeof(glm::vec2));
    }
}

void Cloth::setUpNormals()
{
    m_normals = std::vector<std::vector<glm::vec3>>(m_rows, std::vector<glm::vec3>(m_columns));
    std::vector< std::vector<glm::vec3> > tempNormals[2];
    for (auto i = 0; i < 2; i++) {
        tempNormals[i] = std::vector<std::vector<glm::vec3>>(m_rows - 1, std::vector<glm::vec3>(m_columns - 1));
    }

    for (auto i = 0; i < m_rows - 1; i++)
    {
        for (auto j = 0; j < m_columns - 1; j++)
        {
            const auto& vertexA = m_vertices[i][j].m_position;
            const auto& vertexB = m_vertices[i][j + 1].m_position;
            const auto& vertexC = m_vertices[i + 1][j + 1].m_position;
            const auto& vertexD = m_vertices[i + 1][j].m_position;

            const auto triangleNormalA = glm::cross(vertexB - vertexA, vertexA - vertexD);
            const auto triangleNormalB = glm::cross(vertexD - vertexC, vertexC - vertexB);

            tempNormals[0][i][j] = glm::normalize(triangleNormalA);
            tempNormals[1][i][j] = glm::normalize(triangleNormalB);
        }
    }

    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
            // Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
            // of all adjacent triangles' normals

            const auto isFirstRow = i == 0;
            const auto isFirstColumn = j == 0;
            const auto isLastRow = i == m_rows - 1;
            const auto isLastColumn = j == m_columns - 1;

            auto finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

            // Look for triangle to the upper-left
            if (!isFirstRow && !isFirstColumn) {
                finalVertexNormal += tempNormals[0][i - 1][j - 1];
            }

            // Look for triangles to the upper-right
            if (!isFirstRow && !isLastColumn) {
                for (auto k = 0; k < 2; k++) {
                    finalVertexNormal += tempNormals[k][i - 1][j];
                }
            }

            // Look for triangle to the bottom-right
            if (!isLastRow && !isLastColumn) {
                finalVertexNormal += tempNormals[0][i][j];
            }

            // Look for triangles to the bottom-right
            if (!isLastRow && !isFirstColumn) {
                for (auto k = 0; k < 2; k++) {
                    finalVertexNormal += tempNormals[k][i][j - 1];
                }
            }

            // Store final normal of j-th vertex in i-th row
            m_normals[i][j] = glm::normalize(finalVertexNormal);
        }
        shapesVBO.addRawData(m_normals[i].data(), m_columns * sizeof(glm::vec3));
    }
}

void Cloth::setUpIndexBuffer()
{
    // Create a VBO with heightmap indices
    indexVBO.createVBO();
    indexVBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
    
    for (auto i = 0; i < m_rows - 1; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
            for (auto k = 0; k < 2; k++)
            {
                const auto row = i + k;
                auto index = row * m_columns + j;



            	indexVBO.addRawData(&index, sizeof(int));
            }
        }
        // Restart triangle strips
        indexVBO.addRawData(&m_numVertices, sizeof(int));
    }

    // Send indices to GPU
    indexVBO.uploadDataToGPU(GL_STATIC_DRAW);

    // Calculate total count of indices
    m_numIndices = (m_rows - 1) * m_columns * 2 + m_rows - 1;
}

