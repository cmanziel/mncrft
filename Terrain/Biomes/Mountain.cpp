#include "Mountain.h"

short Mountain::AssignBlockID(unsigned int y, unsigned int colHeight)
{
	short ID;

	if (y >= colHeight)
		ID = air;
	else if (y == colHeight - 1)
		ID = snow;
	else if (y < colHeight - 1 && y > colHeight / 3)
		ID = rock;
	else if (y <= colHeight / 3 && y > colHeight / 6)
		ID = grass_altitude;
	else
		ID = dirt_altitude;

	return ID;
}