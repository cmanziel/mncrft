#include "Biome.h"

//Biome::Biome()
//{
//
//}

biome Biome::Assign(unsigned int colHeight)
{
	if (colHeight > 30)
		return FOREST_BIOME;
	else if (colHeight < 10)
		return BEACH_BIOME;
	else
		return PLAIN_BIOME;
}

short Biome::AssignBlockID(biome colBiome, unsigned int y, unsigned int colHeight)
{
	short ID = sand;

	switch (colBiome)
	{
	case FOREST_BIOME:
		ID = Forest::AssignBlockID(y, colHeight);
		break;
	case BEACH_BIOME:
		ID = Beach::AssignBlockID(y, colHeight);
		break;
	case PLAIN_BIOME:
		ID = Plain::AssignBlockID(y, colHeight);
		break;
	default:
		ID = Plain::AssignBlockID(y, colHeight);
	}

	return ID;
}