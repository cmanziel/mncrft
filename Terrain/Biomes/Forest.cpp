#include "Forest.h"

short Forest::AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height)
{
	short ID;
	int y = position.y;

	if (y >= col_height)
		ID = air;
	else if (y == col_height - 1)
		ID = grass;
	else if (y < chunk_height / 4)
		ID = cobblestone;
	else
		ID = dirt;

	return ID;
}

float Forest::GetBiomeNoise(vec3 position)
{
	float coeff_sum = 1.0f + 0.5f;
	float nv = (NoiseMap::GetValue(position.x, position.z, 0.05f)
		+ 0.5f * NoiseMap::GetValue(position.x, position.z, 0.08f))
		/ coeff_sum;

	return nv;
}