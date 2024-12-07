#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

class Mountain
{
public:
	static short AssignBlockID(unsigned int y, unsigned int colHeight);
private:
};

#endif // !MOUNTAIN_H
