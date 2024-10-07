#ifndef BEACH_H
#define BEACH_H

#include "../../utils/glm_types.h"
#include "../../Noise/NoiseMap.h"
#include "block_IDs.h"

class Beach
{
public:
	static short AssignBlockID(unsigned int y, unsigned int colHeight);
private:
};

#endif // !BEACH_H