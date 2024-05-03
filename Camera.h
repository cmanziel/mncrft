#pragma once

#include "utils/utils.h"

#define CAMERA_MOTION_SPEED 30
#define CAMERA_ROTATION_SPEED 50

class Camera
{
public:
	Camera();
	
	mat4 GetMVP(vec3 blockTrans);
	mat4 GetModelMat4(vec3 blockTrans, int side);
	mat4 GetProjectionMat4();
	mat4 GetViewMat4();

	vec3 GetPosition();
	void Move(int dir);
	void Rotate(int dir);
	void UpdateTime(float dt);

private:
	vec3 m_CameraPos;
	vec3 m_CameraDir;
	vec3 m_CameraFront;
	vec3 m_CameraUp;
	float m_yaw;
	float m_pitch;
	float m_deltaTime;
	float m_TargetDist;
};