#include "Types.h"
#include "Program.h"
#include "BaseCamera.h"

BaseCamera::BaseCamera() :
	m_fieldOfViewY(D3DX_PI * 0.25f),
	m_aspectRatio((float)Program::GetInstance()->GetScreenWidth() / (float)Program::GetInstance()->GetScreenHeight()),
	m_nearPlane(0.1f),
	m_farPlane(5000.0f)
{

}

BaseCamera::~BaseCamera()
{

}

void BaseCamera::SetupCamera(const Vector3D& position, const Vector3D& target, const Vector3D& up)
{
	PositionCamera(position, target, up);
}

void BaseCamera::PositionCamera(const Vector3D& position, const Vector3D& target, const Vector3D& up)
{
	m_position = position;
	m_target = target;
	m_up = up;
}

void BaseCamera::RotateView(const Vector3D& rotate)
{
	Matrix temp;
	MatrixRotationAxis(&temp, &m_right, rotate.y);
	Vector3DTransformCoord(&m_target, &m_target, &temp);
	Vector3DTransformCoord(&m_up, &m_up, &temp);
	MatrixRotationY(&temp, rotate.x);
	Vector3DTransformCoord(&m_right, &m_right, &temp);
	Vector3DTransformCoord(&m_up, &m_up, &temp);
	Vector3DTransformCoord(&m_target, &m_target, &temp);
}

void BaseCamera::SetPosition(const Vector3D& position)
{
	m_position = position;
}

const Vector3D& BaseCamera::GetPosition() const
{
	return m_position;
}

void BaseCamera::SetTarget(const Vector3D& target)
{
	m_target = target;
}

const Vector3D& BaseCamera::GetTarget() const
{
	return m_target;
}

void BaseCamera::SetUp(const Vector3D& up)
{
	m_up = up;
}

const Vector3D& BaseCamera::GetUp() const
{
	return m_up;
}

void BaseCamera::SetFieldofView(float FOV)
{
	m_fieldOfViewY = FOV;
}

float BaseCamera::GetFieldofView() const
{
	return m_fieldOfViewY;
}

void BaseCamera::SetAspectRatio(float ar)
{
	m_aspectRatio = ar;
}

float BaseCamera::GetAspectRatio() const
{
	return m_aspectRatio;
}

void BaseCamera::SetNearPlane(float np)
{
	m_nearPlane = np;
}

float BaseCamera::GetNearPlane() const
{
	return m_nearPlane;
}

void BaseCamera::SetFarPlane(float fp)
{
	m_farPlane = fp;
}

float BaseCamera::GetFarPlane() const
{
	return m_farPlane;
}

void BaseCamera::CreateViewMatrix(Matrix &view)
{
	MatrixLookAtLH(&view, &m_position, &m_target, &m_up);
}

void BaseCamera::CreateProjectionMatrix(Matrix &projection)
{
	MatrixPerspectiveFovLH(&projection, GetFieldofView(), GetAspectRatio(), GetNearPlane(), GetFarPlane());
}