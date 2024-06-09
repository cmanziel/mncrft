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

	void GenerateWorld(Player* player);
  
	void GenerateMeshes(unsigned int chunkNum);

	//gridIndex: vec3 which x and z coords are the indexes for the bidimensional array in memory m_Chunks
	void SetChunkSurroundings(Chunk* chunk, vec3 gridIndex);

	Chunk* m_Chunks[CHUNK_RADIUS * 2 + 1][CHUNK_RADIUS * 2 + 1]; // bi-dimensional array for the chunks' grid

	// index that keeps track of the chunk whose mesh needs to be generated in the current frame.
	unsigned int m_CurrentChunk; // [0, m_Chunks.size() - 1]
	unsigned int m_TotalChunks; // total number of chunks

	// index that keeps track of the chunk whose mesh needs to be generated in the current frame.
	unsigned int m_CurrentChunk; // [0, m_Chunks.size() - 1]

private:
	terrain_buffers* m_Buffers;
};