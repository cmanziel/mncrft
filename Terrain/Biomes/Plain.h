#ifndef PLAIN_H
#define PLAIN_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

// TODO: use noise also for the distribution of trees inside the biome
class Plain
{
public:
	static short AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height);
	static float GetBiomeNoise(vec3 position);
private:
};

#endif // !PLAIN_H