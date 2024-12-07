#include "Grassland.h"

short Grassland::AssignBlockID(unsigned int y, unsigned int colHeight)
{
	if (y > colHeight)
		return air;
	
	return grass;
}