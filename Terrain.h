#pragma once

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include "Player/Player.h"
#include <vector>

#define X_CHUNKS 4
#define Y_CHUNKS 1
#define Z_CHUNKS 4

class Terrain
{
public:
	Terrain(Player* player);
	~Terrain();

	std::vector<Chunk*> GetChunks();

	void GenerateWorld(Player* player);

	void GenerateMeshes(Chunk* chunk, int chunkNum);

	//void SetChunkSurroundings(Chunk* chunk, vec3 chunkLocalPos);
	//void SetChunkSurroundings(Chunk* chunk, size_t indexInTerrain);
	void SetChunkSurroundings(Chunk* chunk, vec3 chunkPlayerIsIn, int indexInTerrain);

	std::vector<Chunk*> m_Chunks;
private:
	vec3 m_Positions[(CHUNK_RADIUS + 1) << 1]; // array of chunk positions, iterate through this array, evaluate which positions are in front of the camera and allocate the respective chunks
	terrain_buffers* m_Buffers;
	unsigned int m_TotalChunks;
	// index that keeps track of th]e chunk whose mesh needs to be generated in the current frame.
	unsigned int m_CurrentChunk; // [0, m_Chunks.size() - 1]
};