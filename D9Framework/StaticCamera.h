#pragma once
#include "BaseCamera.h"

class StaticCamera : public BaseCamera
{
public:
	StaticCamera();
	~StaticCamera();
	virtual const char* GetType() const 
	{ 
		return "STATICCAMERA"; 
	};
	virtual void SetupCamera(const Vector3D& position, const Vector3D& target, const Vector3D& up);
	virtual void CreateViewMatrix(Matrix& view);
	virtual void CreateProjectionMatrix(Matrix& projection);
	virtual void ProcessKeys(float elapsedTime) {}
};

