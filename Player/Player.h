#pragma once

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