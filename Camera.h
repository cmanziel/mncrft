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

	bool IsInsideFrustum(vec3 position); // is the world spcae position passed as argument inside frustum

private:
	vec3 m_CameraPos;
	vec3 m_CameraDir;
	vec3 m_CameraUp;
	vec3 m_CameraFront;

	float m_fovy; // field of view angle on the yz plane
	float m_FocalLength; // distance of the near plane from the camera
	float m_NearToFarDistance;
	float m_NearPlaneLeft; // distance from the m_FocalLength point on the near plane to the left

	float m_yaw;
	float m_pitch;
	float m_deltaTime;
};