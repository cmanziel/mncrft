#ifndef BEACH_H
#define BEACH_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

class Beach
{
public:
	static short AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height);
	static float GetBiomeNoise(vec3 position);
private:
};

#endif // !BEACH_H