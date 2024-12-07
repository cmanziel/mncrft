#ifndef BIOME_H

#define BIOME_H

#include "../../Noise/noise1234.h"
#include "Beach.h"
#include "Forest.h"
#include "Plain.h"
#include "Mountain.h"
#include "Grassland.h"

enum biome
{
	FOREST_BIOME, BEACH_BIOME, PLAIN_BIOME, MOUNTAIN_BIOME, GRASSLAND_BIOME, OCEAN_BIOME
};

class Biome
{
public:
	static biome Assign(unsigned int colHeight);
	static short AssignBlockID(biome colBiome, unsigned int y, unsigned int colHeight);
private:
};

#endif // !BIOME_H