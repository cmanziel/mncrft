#include "NoiseMap.h"
#include "perlin.h"

NoiseMap::NoiseMap(int chunkSize)
	: m_ChunkSize(chunkSize)
{
	//m_GridValues = (int*)malloc(sizeof(int) * chunkSize * chunkSize);
	m_GridValues = (float**)malloc(sizeof(float*) * chunkSize);

	if (m_GridValues == NULL)
	{
		printf("noise map grid not allocated");
		return;
	}

	GenerateMap();
}

// copy constructor
NoiseMap::NoiseMap(NoiseMap& other)
{
	m_ChunkSize = other.m_ChunkSize;

	m_GridValues = (float**)malloc(sizeof(float*) * m_ChunkSize);

	if (m_GridValues == NULL)
	{
		printf("noise map grid not allocated");
		return;
	}

	// copy the noise map of other in the noise map of this
	for (int y = 0; y < m_ChunkSize; y++)
	{
		m_GridValues[y] = (float*)malloc(sizeof(float) * m_ChunkSize);

		if (m_GridValues[y] == NULL)
		{
			printf("noise grid not allocated");
			return;
		}

		for (int x = 0; x < m_ChunkSize; x++)
		{
			m_GridValues[y][x] = other.m_GridValues[y][x];
		}
	}
}

void NoiseMap::GenerateMap()
{
	for (int y = 0; y < m_ChunkSize; y++)
	{
		m_GridValues[y] = (float*)malloc(sizeof(float) * m_ChunkSize);

		if (m_GridValues[y] == NULL)
		{
			printf("noise grid not allocated");
			return;
		}

		for (int x = 0; x < m_ChunkSize; x++)
		{
			m_GridValues[y][x] = perlin(x + 0.5, y + 0.5);
		}
	}
}

float** NoiseMap::GetGridValues()
{
	return m_GridValues;
}

NoiseMap& NoiseMap::operator= (NoiseMap& other)
{
	if (this == &other)
		return *this;

	m_ChunkSize = other.m_ChunkSize;

	for (int y = 0; y < m_ChunkSize; y++)
	{
		for (int x = 0; x < m_ChunkSize; x++)
		{
			m_GridValues[y][x] = other.m_GridValues[y][x];
		}
	}

	return *this;
}

NoiseMap::~NoiseMap()
{
	// free pointer of pointers
	for (int i = 0; i < m_ChunkSize; i++)
	{
		free(m_GridValues[i]);
	}

	free(m_GridValues);
}