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

#include "../Camera.h"
#include "../Chunk/Chunk.h"

#define CHUNK_RADIUS 5

class Player
{
public:
	Player();
	int GetChunkRadius();
	Camera* GetCam();
	void UpdateChunkGridPosition();

	vec3 GetChunkGridPosition();

	vec3 GetLastChunkGridPosition();
	void SetLastChunkGridPosition();

	~Player();

private:
	Camera* m_PlayerCam;
	int m_ChunkRadius;
	vec3 m_ChunkGridPosition;
	vec3 m_LastGridPosition;
};