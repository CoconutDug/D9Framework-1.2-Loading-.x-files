#include "Types.h"
#include "Program.h"
#include "StaticCamera.h"

StaticCamera::StaticCamera()
{
}

StaticCamera::~StaticCamera()
{
}

void StaticCamera::SetupCamera(const Vector3D& position, const Vector3D& target, const Vector3D& up)
{
	m_position = position;
	m_target = target;
	m_up = up;
	Vector3DCrossProduct(&m_right, &m_up, &m_target);
}

void StaticCamera::CreateViewMatrix(Matrix &view)
{
	MatrixLookAtLH(&view, &m_position, &m_target, &m_up);
}

void StaticCamera::CreateProjectionMatrix(Matrix &projection)
{
	MatrixPerspectiveFovLH(&projection, GetFieldofView(), GetAspectRatio(), GetNearPlane(), GetFarPlane());

}