#pragma once
#include "Object.h"
#include "Particle.h"
class Terrain :
    public Object
{
public:

	Object* cube;
	float m_fanSpeed = 0.09f;

	int getVertexByteSize()
	{
		return sizeof(glm::vec3) * 2 + sizeof(glm::vec2);
	}

	Terrain();
	
	void init(Camera* _cam);
	void createFromHeightData();

	void Render();
	void move(const std::function<bool(int)>& keyInputFunc);
	void Update(float _dT);
	void RenderPoints();

	int getRows() const;
	int getColumns() const;
	float getHeight(const int _row, const int _column) const;


	Particle* renderedPosition(float x, float z);


private:
	void setUpVertices(bool sec = false);
	void setUpTextureCoordinates();
	void setUpNormals();
	void setUpIndexBuffer();

	glm::mat3 normalMatrix;
	
	std::vector <std::vector<float>> m_heightData;
	std::vector <std::vector<Particle>> m_vertices;
	std::vector <std::vector<glm::vec2>> m_textureCoordinates;
	std::vector <std::vector<glm::vec3>> m_normals;
	int m_rows = 0;
	int m_columns = 0;
	bool m_isInitialized = false;
	int m_numVertices;
	int m_numIndices = 0;
	bool flag;
};
