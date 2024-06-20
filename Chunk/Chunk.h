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

#include "../Block/Block.h"
#include "../Noise/NoiseMap.h"
#include "../Mesh/Mesh.h"
#include "../Player/Player.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 20

// these indexes should map the block "sides" enum indexes so that the function AddBlockToMesh functions properly when looping through a block's faces
enum surr_chunks {
	left_chunk, right_chunk, front_chunk, back_chunk
};

class Mesh; // forward declaration

/*
	* chunk class needs m_Blocks field because every chunk hass a different structure initially and also it can be modified
	* but the blocks don't need to store neither faces nor vertices because they are all the same for each one of them
*/

class Chunk
{
public:
	Chunk(vec3 position, Player* player, unsigned int offsetIntoBuffer);
	Chunk(Chunk& other);
	Chunk& operator= (Chunk& other);
	~Chunk();
	
	Block* GetBlock(vec3 position);
	vec3 GetPosition();

	Player* GetPlayer();

	NoiseMap* GetNoiseMap();

	void BuildMesh(terrain_buffers* terrainBufs);
	Mesh* GetMesh();
	void AddBlockToMesh(Block* block);
	bool IsAdjacentBlockSolid(Block* block, vec3 adjBlockPos, Chunk* adjChunk);

	std::vector<Block*> GetBlocksVector();

	unsigned int GetOffsetIntoBuffer();
	unsigned int GetLowestSolidHeight();

	//surr_chunks* m_Surrounding;
	//surr_chunks* GetSurrounding();
	void SetSurroundings(Chunk** surr);

	Chunk** GetSurrounding();

	unsigned int m_LowestSolidHeight;
private:
	vec3 m_Position;
	NoiseMap* m_NoiseMap;
	Player* m_Player;
	Chunk* m_Surrounding[4]; // array of chunks
	std::vector<Block*> m_Blocks;
	Mesh* m_Mesh;
	unsigned int m_OffsetIntoBuffer;
};