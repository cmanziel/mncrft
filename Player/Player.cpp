#include "Player.h"

Player::Player()
{
	Camera* cam = new Camera();

	m_PlayerCam = cam;

	// CHUNK_RADIUS DEFINED IN Player.h
	// render CHUNK_RADIUS number of chunks in the positive and negative x and z directions around the player
	m_ChunkRadius = CHUNK_RADIUS;

	vec3 playerPos = m_PlayerCam->GetPosition();

	vec3 chunkPlayerIsIn = vec3((int)(playerPos.x / CHUNK_SIZE), 0, (int)(playerPos.z / CHUNK_SIZE));

	m_ChunkGridPosition = chunkPlayerIsIn;

	// when player gets created last Position is the player's position
	m_LastGridPosition = m_ChunkGridPosition;
}

int Player::GetChunkRadius()
{
	return m_ChunkRadius;
}

Camera* Player::GetCam()
{
	return m_PlayerCam;
}

void Player::UpdateChunkGridPosition()
{
	vec3 playerPos = m_PlayerCam->GetPosition();

	// need to floor the x and z coordinate floating point value
	// otherwise for the coordinates less than zero the number gets truncated to the greater integer and the change between chunks in the grid is not detected

	int xGridCoord = floor(playerPos.x / CHUNK_SIZE);
	int zGridCoord = floor(playerPos.z / CHUNK_SIZE);

	vec3 chunkPlayerIsIn = vec3(xGridCoord, 0, zGridCoord);

	m_ChunkGridPosition = chunkPlayerIsIn;
}

vec3 Player::GetChunkGridPosition()
{
	return m_ChunkGridPosition;
}

vec3 Player::GetLastChunkGridPosition()
{
	return m_LastGridPosition;
}

void Player::SetLastChunkGridPosition()
{
	m_LastGridPosition = m_ChunkGridPosition;
}

Player::~Player()
{
	delete m_PlayerCam;
}