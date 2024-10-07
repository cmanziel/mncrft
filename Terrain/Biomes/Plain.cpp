#include "Plain.h"

short Plain::AssignBlockID(unsigned int y, unsigned int colHeight)
{
	short ID;

	if (y >= colHeight)
		ID = air;
	else
		ID = dirt;

	return ID;
}