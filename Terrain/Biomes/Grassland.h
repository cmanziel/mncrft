#ifndef GRASSLAND_H
#define GRASSLAND_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

class Grassland
{
public:
	static short AssignBlockID(unsigned int y, unsigned int colHeight);
private:
};

#endif // !GRASSLAND_H
