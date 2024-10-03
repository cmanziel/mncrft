#ifndef FOREST_H
#define FOREST_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

// TODO: use noise also for the distribution of trees inside the biome
class Forest
{
public:
	static short AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height);
	static float GetBiomeNoise(vec3 position);
private:
};

#endif // !FOREST_H