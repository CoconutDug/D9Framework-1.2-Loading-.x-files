#pragma once

#include "BaseCamera.h"

class ThirdPersonCamera : public BaseCamera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();
	virtual const char*	GetType() const { return "THIRDPERSON"; };
	virtual void CreateViewMatrix(Matrix& view);
	virtual void CreateProjectionMatrix(Matrix& projection);
	virtual void ProcessKeys(float elapsedTime);
	void SetDistanceFromObject(float distance);
	void SetUpThirdPersonCamera(float yaw, float pitch);
	void SetTargetYaw(float yaw);
private:
	float m_distance;
	float m_targetDistance;
	float m_yaw;
	float m_targetYaw;
	float m_pitch;
	float m_targetPitch;
	float m_intersectionTime;
	bool m_tendTo;
};
