#include "Forest.h"

//short Forest::AssignBlockID(vec3 position, unsigned int col_height, unsigned int chunk_height)
//{
//	short ID;
//	int y = position.y;
//
//	if (y >= col_height)
//		ID = air;
//	else if (y == col_height - 1)
//		ID = grass;
//	else if (y < chunk_height / 4)
//		ID = cobblestone;
//	else
//		ID = dirt;
//
//	return ID;
//}

short Forest::AssignBlockID(unsigned int y, unsigned int colHeight)
{
	short ID;

	if (y >= colHeight)
		ID = air;
	else if (y == colHeight - 1)
		ID = grass;
	else if (y < colHeight - 1 && y > colHeight / 3)
		ID = dirt;
	else
		ID = cobblestone;

	return ID;
}