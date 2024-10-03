#include "NoiseMap.h"
#include "perlin.h"

NoiseMap::NoiseMap(int size)
	: m_Size(size)
{
	//m_GridValues = (int*)malloc(sizeof(int) * chunkSize * chunkSize);
	m_GridValues = (float**)malloc(sizeof(float*) * size);

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
	m_Size = other.m_Size;

	m_GridValues = (float**)malloc(sizeof(float*) * m_Size);

	if (m_GridValues == NULL)
	{
		printf("noise map grid not allocated");
		return;
	}

	// copy the noise map of other in the noise map of this
	for (int y = 0; y < m_Size; y++)
	{
		m_GridValues[y] = (float*)malloc(sizeof(float) * m_Size);

		if (m_GridValues[y] == NULL)
		{
			printf("noise grid not allocated");
			return;
		}

		for (int x = 0; x < m_Size; x++)
		{
			m_GridValues[y][x] = other.m_GridValues[y][x];
		}
	}
}

void NoiseMap::GenerateMap()
{
	for (int y = 0; y < m_Size; y++)
	{
		m_GridValues[y] = (float*)malloc(sizeof(float) * m_Size);

		if (m_GridValues[y] == NULL)
		{
			printf("noise grid not allocated");
			return;
		}

		for (int x = 0; x < m_Size; x++)
		{
			m_GridValues[y][x] = perlin(x + 0.5f, y + 0.5f);
		}
	}
}

float** NoiseMap::GetGridValues()
{
	return m_GridValues;
}

float NoiseMap::GetValue(float x, float y, float frequency)
{
	return perlin((x + 0.5f) * frequency, (y + 0.5f) * frequency);
}

NoiseMap& NoiseMap::operator= (NoiseMap& other)
{
	if (this == &other)
		return *this;

	m_Size = other.m_Size;

	for (int y = 0; y < m_Size; y++)
	{
		for (int x = 0; x < m_Size; x++)
		{
			m_GridValues[y][x] = other.m_GridValues[y][x];
		}
	}

	return *this;
}

NoiseMap::~NoiseMap()
{
	// free pointer of pointers
	for (int i = 0; i < m_Size; i++)
	{
		free(m_GridValues[i]);
	}

	free(m_GridValues);
}