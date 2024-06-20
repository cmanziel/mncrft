#include "Player.h"

// avoid circular dependency between player and chunk classes by passing chunk parameters to player

Player::Player()
{
	m_WorldPosition = vec3(0.0, 0.0, 0.0);
	m_BreakMaxDistance = 5.0;
	m_State = STATE_WATCH;

	Camera * cam = new Camera(m_WorldPosition);
	m_Ray = Ray(m_WorldPosition, cam->GetCameraFront());

	m_PlayerCam = cam;

	m_ChunkGridPosition = vec3(0, 0, 0);

	m_LastGridPosition = m_ChunkGridPosition;
}

Player::~Player()
{
	delete m_PlayerCam;
	//delete m_Ray;
}

Camera* Player::GetCam()
{
	return m_PlayerCam;
}

vec3 Player::GetWorldPosition()
{
	return m_WorldPosition;
}

uint8_t Player::GetState()
{
	return m_State;
}

void Player::ChangeState()
{
	m_State = m_State != STATE_BREAK ? STATE_BREAK : STATE_WATCH;
}

void Player::UpdateChunkGridPosition(vec3 chunkGridPosition)
{
	// update also position based on the camera's in order to get the Ray right
	m_WorldPosition = m_PlayerCam->GetPosition();

	m_ChunkGridPosition = chunkGridPosition;
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