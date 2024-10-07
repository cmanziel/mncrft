#ifndef FOREST_H
#define FOREST_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

// TODO: use noise also for the distribution of trees inside the biome
class Forest
{
public:
	static short AssignBlockID(unsigned int y, unsigned int colHeight);
private:
};

#endif // !FOREST_H