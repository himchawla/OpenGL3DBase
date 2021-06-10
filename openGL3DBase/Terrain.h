#pragma once
#include "Object.h"
class Terrain :
    public Object
{
public:
	struct HillAlgorithmParameters
	{
		HillAlgorithmParameters(int rows, int columns, int numHills, int hillRadiusMin, int hillRadiusMax, float hillMinHeight, float hillMaxHeight)
		{
			this->rows = rows;
			this->columns = columns;
			this->numHills = numHills;
			this->hillRadiusMin = hillRadiusMin;
			this->hillRadiusMax = hillRadiusMax;
			this->hillMinHeight = hillMinHeight;
			this->hillMaxHeight = hillMaxHeight;
		}

		int rows;
		int columns;
		int numHills;
		int hillRadiusMin;
		int hillRadiusMax;
		float hillMinHeight;
		float hillMaxHeight;
	};

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

	static std::vector <std::vector<float>> generateRandomHeightData(const HillAlgorithmParameters& params);

	float getRenderedHeightAtPosition(const glm::vec3& renderSize, const glm::vec3& position);

private:
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

