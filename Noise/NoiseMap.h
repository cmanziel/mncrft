#pragma once

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

	~NoiseMap();

private:
	float** m_GridValues;
	int m_ChunkSize;
};

//every chunk has a noisemap field
//m_GridValues containes the different perlin noise values for every point of the grid of columns of the row
//
//imagine this as a top-down view of a chunk where every square of the gird is the top block of a column
//a value is computed for each square and that values determines the scale of the CHUNK_HEIGHT value that is given to the column