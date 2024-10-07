#include "Beach.h"

short Beach::AssignBlockID(unsigned int y, unsigned int colHeight)
{
	short ID;

	if (y >= colHeight)
		ID = air;
	else
		ID = sand;

	return ID;
}