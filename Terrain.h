#pragma once

#include <vector>
#include "Player/Player.h"

#define X_CHUNKS 4
#define Y_CHUNKS 1
#define Z_CHUNKS 4

class Terrain
{
public:
	Terrain(Player* player);
	~Terrain();

	void GenerateWorld(Player* player, int chunkNum);

	void GenerateMeshes(Chunk* chunk, int chunkNum);

	//void SetChunkSurroundings(Chunk* chunk, vec3 chunkLocalPos);
	//void SetChunkSurroundings(Chunk* chunk, size_t indexInTerrain);
	void SetChunkSurroundings(Chunk* chunk, vec3 chunkPlayerIsIn, int indexInTerrain);
	std::vector<Chunk*> m_Chunks;

private:
	terrain_buffers* m_Buffers;
};