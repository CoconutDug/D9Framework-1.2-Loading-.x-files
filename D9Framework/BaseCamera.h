#pragma once
#include "Types.h"

class Frustum;
class BaseCamera
{
public:
	BaseCamera();
	virtual ~BaseCamera();
	virtual const char*	GetType() const = 0;
	virtual void ProcessKeys(float elapsedTime) = 0;
	virtual void SetupCamera(const Vector3D& position, const Vector3D& at, const Vector3D& up);
	virtual void CreateViewMatrix(Matrix& view);
	virtual void CreateProjectionMatrix(Matrix& projection);
	void PositionCamera(const Vector3D& position, const Vector3D& at, const Vector3D& up);
	void RotateView(const Vector3D& rotate);
	void SetPosition(const Vector3D& position);
	const Vector3D& GetPosition() const;
	void SetTarget(const Vector3D& target);
	const Vector3D&	GetTarget() const;
	void SetUp(const Vector3D& up);
	const Vector3D&	GetUp() const;
	void SetFieldofView(float FOV);
	float GetFieldofView() const;
	void SetAspectRatio(float ar);
	float GetAspectRatio() const;
	void SetNearPlane(float np);
	float GetNearPlane() const;
	void SetFarPlane(float fp);
	float GetFarPlane() const;
protected:
	Vector3D m_position;
	Vector3D m_target;
	Vector3D m_up;
	Vector3D m_right;
	float m_fieldOfViewY;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
};
