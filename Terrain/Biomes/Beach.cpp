#include "Beach.h"

float Beach::GetBiomeNoise(vec3 position)
{
	float nv = NoiseMap::GetValue(position.x, position.z, 0.05f);

	return nv;
}

short Beach::AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height)
{
	short ID;
	int y = position.y;

	if (y >= col_height)
		ID = air;
	else
		ID = sand;

	return ID;
}