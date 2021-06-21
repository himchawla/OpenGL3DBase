#include "Terrain.h"

#include <stb_image.h>

#include "Headers.h"

float Terrain::random(int x, int y)
{
	int n = x + y * seed;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t) * 0.931322574615478515625e-9;
}

float Terrain::interpolate(float a, float b, float x)
{
	return a * (1 - x) + b * x;
}

float Terrain::noise(float x, float y)
{
	float fractional_X = x - int(x);
	float fractional_Y = y - int(y);

	//smooths
	float v1 = smooth(int(x), int(y));
	float v2 = smooth(int(x) + 1, int(y));
	float v3 = smooth(int(x), int(y) + 1);
	float v4 = smooth(int(x) + 1, int(y) + 1);

	// interpolates
	float i1 = interpolate(v1, v2, fractional_X);
	float i2 = interpolate(v3, v4, fractional_X);

	return interpolate(i1, i2, fractional_Y);
}

float Terrain::totalNoisePerPoint(int x, int y)
{
	int octaves = 8;
	float zoom = 20.0f;
	float persistence = 0.6f;
	float total = 0.0f;

	for (int i = 0; i < octaves - 1; i++)
	{
		float frequency = pow(2, i) / zoom;
		float amplitude = pow(persistence, i);

		total += noise(x * frequency, y * frequency) * amplitude;
	}
	return total;
}

float Terrain::smooth(int x, int y)
{
	float corners;
	float sides;
	float center;

	corners = (random(x - 1, y - 1) + random(x + 1, y - 1) + random(x - 1, y + 1) + random(x + 1, y + 1)) / 16;
	sides = (random(x - 1, y) + random(x + 1, y) + random(x, y - 1) + random(x, y + 1)) / 8;
	center = random(x, y) / 4;
	return corners + sides + center;
}

Terrain::Terrain()
		:Object()
{
	srand(time(0));
	seed = rand() % 100;
	std::cout << seed;
}

void Terrain::init(Camera* _cam)
{
    Object::init("vertex.vert", "fragment.frag", _cam, 0);
    createFromHeightData(generateRandomHeightData(HillAlgorithmParameters(256,256)));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
}

void Terrain::createFromHeightData(const std::vector<std::vector<float>>& _heightData)
{
    if (m_isInitialized) {
        shapesVBO.deleteVBO();
    }

    m_heightData = _heightData;
    m_rows = static_cast<int>(_heightData.size());
    m_columns = static_cast<int>(_heightData[0].size());
    m_numVertices = m_rows * m_columns;

    // First, prepare VAO and VBO for vertex data
    glGenVertexArrays(1, &mainVAO);
    glBindVertexArray(mainVAO);
    shapesVBO.createVBO(m_numVertices * getVertexByteSize()); // Preallocate memory
    shapesVBO.bindVBO();
    setUpVertices();
    setUpTextureCoordinates();
    setUpNormals();

    // Send data to GPU, they're ready now
    shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

    setVertexAttributesPointers(m_numVertices);

    // Vertex data are in, set up the index buffer
    setUpIndexBuffer();

    // Clear the data, we won't need it anymore
    m_vertices.clear();
    m_textureCoordinates.clear();
    m_normals.clear();

    // If get here, we have succeeded with generating heightmap
    m_isInitialized = true;
}

void Terrain::Render()
{
    if (!m_isInitialized) {
        return;
    }
    program.useProgram();
    glCullFace(GL_BACK);
	glBindVertexArray(mainVAO);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(m_numVertices);
    transform.position.x = 0.0f;
    program["PVM"] = camera->Project(glm::scale(modelMatrix, glm::vec3(128.0f)));
    program["gSampler"] = 0;
    texture.bind();

    glDrawElements(GL_TRIANGLE_STRIP, m_numIndices, GL_UNSIGNED_INT, 0);
    glDisable(GL_PRIMITIVE_RESTART);
}


void Terrain::RenderPoints()
{
    if (!m_isInitialized) {
        return;
    }

    glBindVertexArray(mainVAO);

    // Render points only
    glDrawArrays(GL_POINTS, 0, m_numVertices);
}


std::vector<std::vector<float>> Terrain::getHeightDataFromImage(const std::string& fileName)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, bytesPerPixel;
    const auto imageData = stbi_load(fileName.c_str(), &width, &height, &bytesPerPixel, 0);
    if (imageData == nullptr)
    {
        // Return empty vector in case of failure
        std::cout << "Failed to load heightmap image " << fileName << "!" << std::endl;
        return std::vector<std::vector<float>>();
    }

    std::vector<std::vector<float>> result(height, std::vector<float>(width));
    auto pixelPtr = &imageData[0];
    for (auto i = 0; i < height; i++)
    {
        for (auto j = 0; j < width; j++)
        {
            result[i][j] = static_cast<float>(*pixelPtr) / 255.0f;
            pixelPtr += bytesPerPixel;
        }
    }

    stbi_image_free(imageData);
    return result;
}


int Terrain::getRows() const
{
    return m_rows;
}

int Terrain::getColumns() const
{
    return m_columns;
}

float Terrain::getHeight(const int _row, const int _column) const
{
    if (_row < 0 || _row >= m_rows || _column < 0 || _column >= m_columns) {
        return 0.0f;
    }

    return m_heightData[_row][_column];
}

float Terrain::getRenderedHeightAtPosition(const glm::vec3& renderSize, const glm::vec3& position)
{
    const auto halfWidth = renderSize.x / 2.0f;
    const auto halfDepth = renderSize.z / 2.0f;

    const auto row = static_cast<int>(m_rows * (position.z + halfDepth) / renderSize.z);
    const auto column = static_cast<int>(m_columns * (position.x + halfWidth) / renderSize.x);

    return getHeight(row, column) * renderSize.y;
}




std::vector<std::vector<float>> Terrain::generateRandomHeightData(const HillAlgorithmParameters& params)
{
	int y = rand() % 100;
	int x = rand() % 100;

	m_rows = params.rows;
	m_columns = params.columns;

	m_heightData.resize(m_rows);
	FOR(i, m_rows) m_heightData[i].resize(m_columns);

	FOR(i, m_rows) FOR(j, m_columns) m_heightData[i][j] = totalNoisePerPoint(i, j);

	return m_heightData;

	
	
}

void Terrain::setUpVertices()
{
    m_vertices = std::vector<std::vector<glm::vec3>>(m_rows, std::vector<glm::vec3>(m_columns));

    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
			m_heightData[i][j] /= 10.0f;  
            const auto factorRow = static_cast<float>(i) / static_cast<float>(m_rows - 1);
            const auto factorColumn = static_cast<float>(j) / static_cast<float>(m_columns - 1);
            const auto& fVertexHeight = m_heightData[i][j];
            m_vertices[i][j] = glm::vec3(-0.5f + factorColumn, fVertexHeight, -0.5f + factorRow);
        }
        shapesVBO.addRawData(m_vertices[i].data(), m_columns * sizeof(glm::vec3));
    }
}

void Terrain::setUpTextureCoordinates()
{
    m_textureCoordinates = std::vector<std::vector<glm::vec2>>(m_rows, std::vector<glm::vec2>(m_columns));

    const auto textureStepU = 0.1f;
    const auto textureStepV = 0.1f;

    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++) {
            m_textureCoordinates[i][j] = glm::vec2(textureStepU * j, textureStepV * i);
        }
        shapesVBO.addRawData(m_textureCoordinates[i].data(), m_columns * sizeof(glm::vec2));
    }
}

void Terrain::setUpNormals()
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
            const auto& vertexA = m_vertices[i][j];
            const auto& vertexB = m_vertices[i][j + 1];
            const auto& vertexC = m_vertices[i + 1][j + 1];
            const auto& vertexD = m_vertices[i + 1][j];

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

void Terrain::setUpIndexBuffer()
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

