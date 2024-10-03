#ifndef BIOME_H

#define BIOME_H

#include "Beach.h"
#include "Forest.h"
#include "Plain.h"

enum types
{
	FOREST_BIOME, BEACH_BIOME, PLAIN_BIOME
};

class Biome
{
public:
	Biome(vec3 position);
	~Biome() = default;

	Biome& operator=(Biome& other);

	short AssignBlockID(vec3 position, unsigned int colHeight, unsigned int chunkHeight); // col_height: height of the column of solid blocks of the curretn block whose id is being assigned
	float GetBiomeNoise(vec3 position);
private:
	short m_Type;
};

#endif // !BIOME_H