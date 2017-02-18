#pragma once
#include "BaseCamera.h"

class FreeCamera : public BaseCamera
{
public:
	FreeCamera();
	~FreeCamera();
	virtual const char* GetType() const 
	{ 
		return "FREECAM"; 
	};
	virtual void CreateViewMatrix(Matrix& view);
	virtual void CreateProjectionMatrix(Matrix& projection);
	virtual void ProcessKeys(float elapsedTime);
private:
	float m_angle;
	float m_pitch;
	float m_speed;
	float m_xRot;
	float m_yRot;
};
