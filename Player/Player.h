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

#include "../Camera/Camera.h"

#define CHUNK_RADIUS 5

enum state
{
	STATE_WATCH, STATE_BREAK
};

class Player
{
public:
	Player();
	~Player();

	uint8_t GetState();
	void ChangeState();

	vec3 GetWorldPosition();

	void UpdateChunkGridPosition(vec3 chunkGridPos);
	vec3 GetChunkGridPosition();

	vec3 GetLastChunkGridPosition();
	void SetLastChunkGridPosition();

	Camera* GetCam();

	Ray m_Ray;
	float m_BreakMaxDistance; // maximum distance from a block to be able to break it

private:
	Camera* m_PlayerCam;

	uint8_t m_State;

	vec3 m_WorldPosition;

	vec3 m_ChunkGridPosition;
	vec3 m_LastGridPosition;
};