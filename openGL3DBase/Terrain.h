#pragma once
#include "Object.h"
class Terrain :
	public Object
{
public:
	struct HillAlgorithmParameters
	{
		HillAlgorithmParameters(int rows, int columns)
		{
			this->rows = rows;
			this->columns = columns;
		}

		int rows;
		int columns;
		
	};

	float random(int x, int y);

	float interpolate(float a, float b, float x);


	float noise(float x, float y);

	float totalNoisePerPoint(int x, int y);


	float smooth(int x, int y);


	int getVertexByteSize()
	{
		return sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
	}

	Terrain();
	
	void init(Camera* _cam);
	void createFromHeightData(const std::vector <std::vector<float>>& _heightData);

	void Render();
	void RenderPoints();
	std::vector<std::vector<float>> getHeightDataFromImage(const std::string& fileName);

	int getRows() const;
	int getColumns() const;
	float getHeight(const int _row, const int _column) const;

	std::vector <std::vector<float>> generateRandomHeightData(const HillAlgorithmParameters& params);

	float getRenderedHeightAtPosition(const glm::vec3& renderSize, const glm::vec3& position);

private:

	float seed;
	void setUpVertices();
	void setUpTextureCoordinates();
	void setUpNormals();
	void setUpIndexBuffer();

	glm::mat3 normalMatrix;
	
	std::vector <std::vector<float>> m_heightData;
	std::vector <std::vector<glm::vec3>> m_vertices;
	std::vector <std::vector<glm::vec2>> m_textureCoordinates;
	std::vector <std::vector<glm::vec3>> m_normals;
	int m_rows = 0;
	int m_columns = 0;
	bool m_isInitialized = false;
	int m_numVertices;
	int m_numIndices = 0;
};

