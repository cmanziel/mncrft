#include "Biome.h"

Biome::Biome(vec3 position)
{

}

Biome& Biome::operator=(Biome& other)
{
	m_Type = other.m_Type;

	return *this;
}

short Biome::AssignBlockID(vec3 position, unsigned int colHeight, unsigned int chunkHeight)
{
	short ID = sand;

	switch (m_Type)
	{
	case FOREST_BIOME:
		ID = Forest::AssignBlockID(position, colHeight, chunkHeight);
		break;
	case BEACH_BIOME:
		ID = Beach::AssignBlockID(position, colHeight, chunkHeight);
		break;
	case PLAIN_BIOME:
		ID = Plain::AssignBlockID(position, colHeight, chunkHeight);
		break;
	default:
		Plain::AssignBlockID(position, colHeight, chunkHeight);
	}

	return sand;

	return ID;
}

float Biome::GetBiomeNoise(vec3 position)
{
	float nv; // default in case of error

	switch (m_Type)
	{
	case FOREST_BIOME:
		nv = Forest::GetBiomeNoise(position);
		break;
	case BEACH_BIOME:
		nv = Beach::GetBiomeNoise(position);
		break;
	case PLAIN_BIOME:
		nv = Plain::GetBiomeNoise(position);
		break;
	default:
		nv = Plain::GetBiomeNoise(position);
	}

	return nv;
}