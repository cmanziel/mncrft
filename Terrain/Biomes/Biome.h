#ifndef BIOME_H

#define BIOME_H

#include "../../Noise/noise1234.h"
#include "Beach.h"
#include "Forest.h"
#include "Plain.h"

enum biome
{
	FOREST_BIOME, BEACH_BIOME, PLAIN_BIOME
};

class Biome
{
public:
	static biome Assign(unsigned int colHeight);
	static short AssignBlockID(enum biome colBiome, unsigned int y, unsigned int colHeight);
private:
};

#endif // !BIOME_H