#pragma once

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <stdio.h>
#include <stdlib.h>


class NoiseMap
{
public:
	NoiseMap(int chunkSize);
	NoiseMap(NoiseMap& other);

	NoiseMap& operator= (NoiseMap& other);

	void GenerateMap();
	float** GetGridValues();

	static float GetValue(float x, float y, float frequency);

	~NoiseMap();

	static float m_BaseFrequency;
private:
	float** m_GridValues;
	int m_Size;
};

//every chunk has a noisemap field
//m_GridValues containes the different perlin noise values for every point of the grid of columns of the row
//
//imagine this as a top-down view of a chunk where every square of the gird is the top block of a column
//a value is computed for each square and that values determines the scale of the CHUNK_HEIGHT value that is given to the column