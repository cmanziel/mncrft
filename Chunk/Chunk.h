#pragma once

#include "../Block.h"
#include "../Noise/NoiseMap.h"
#include "../Mesh.h"

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 20

enum surr_chunks {
	left_chunk, right_chunk, back_chunk, front_chunk
};

class Mesh; // forward declaration
class Player;

/*
	* chunk class needs m_Blocks field because every chunk hass a different structure initially and also it can be modified
	* but the blocks don't need to store neither faces nor vertices because they are all the same for each one of them
*/

class Chunk
{
public:
	Chunk(vec3 position, Player* player, unsigned int offsetIntoBuffer);
	Chunk(Chunk& other, Player* player);

	Chunk& operator= (Chunk& other);

	~Chunk();
	
	Block* GetBlock(vec3 position);
	vec3 GetPosition();

	Player* GetPlayer();
	NoiseMap* GetNoiseMap();

	Mesh* GetMesh();
	std::vector<Block*> GetBlocksVector();

	unsigned int GetOffsetIntoBuffer();
	unsigned int GetLowestSolidHeight();

	//surr_chunks* m_Surrounding;
	//surr_chunks* GetSurrounding();
	void SetPosition(vec3 pos);
	void SetSurroundings(Chunk** surr);

	Chunk** GetSurrounding();

private:
	vec3 m_Position;
	NoiseMap* m_NoiseMap;
	Chunk** m_Surrounding; // array of chunks
	std::vector<Block*> m_Blocks;
	Mesh* m_Mesh;
	Player* m_Player;
	unsigned int m_OffsetIntoBuffer;
	unsigned int m_LowestSolidHeight;
};