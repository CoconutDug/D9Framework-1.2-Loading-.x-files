#include "FreeCamera.h"
#include "Program.h"
#include "InputManager.h"

FreeCamera::FreeCamera(void)
{
	m_position = Vector3D(0, 0, -5);
	m_target = Vector3D(0, 0, 1);
	m_up = Vector3D(0, 1, 0);
	m_right = Vector3D(1, 0, 0);
	m_speed = 2;
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::ProcessKeys(float elapsedTime)
{
	if (InputManager::GetInstance()->IsMouseButtonDown(1))
	{
		RotateView(Vector3D(InputManager::GetInstance()->GetMouseX() * 0.003f, InputManager::GetInstance()->GetMouseY() * 0.003f, 0.0f));
	}

	Vector3D direction(0, 0, 0);
	if (InputManager::GetInstance()->IsKeyDown(false, VK_LCONTROL))
	{
		m_speed = 8;
	}
	else
	{
		m_speed = 2;
	}

	if (InputManager::GetInstance()->IsKeyDown(true, 'w'))
	{
		direction += m_target*m_speed;
	}
	else if (InputManager::GetInstance()->IsKeyDown(true, 's'))
	{
		direction -= m_target*m_speed;
	}

	if (InputManager::GetInstance()->IsKeyDown(true, 'd'))
	{
		direction += m_right*m_speed;
	}
	else if (InputManager::GetInstance()->IsKeyDown(true, 'a'))
	{
		direction -= m_right*m_speed;
	}

	Vector3DNormalize(&direction, &direction);
	m_position += direction * 10 * elapsedTime;
}

void FreeCamera::CreateViewMatrix(Matrix &view)
{
	if (m_target.y < -0.7f)
	{
		m_target.y = -0.7f;
	}
	else if (m_target.y > 0.7f)
	{
		m_target.y = 0.7f;
	}

	m_right.y = 0;
	Vector3DNormalize(&m_target, &m_target);
	Vector3DCrossProduct(&m_up, &m_target, &m_right);
	Vector3DNormalize(&m_up, &m_up);

	Vector3DCrossProduct(&m_right, &m_up, &m_target);
	Vector3DNormalize(&m_right, &m_right);

	float x = -Vector3DDotProduct(&m_position, &m_right);
	float y = -Vector3DDotProduct(&m_position, &m_up);
	float z = -Vector3DDotProduct(&m_position, &m_target);

	view(0, 0) = m_right.x;
	view(1, 0) = m_right.y;
	view(2, 0) = m_right.z;
	view(3, 0) = x;

	view(0, 1) = m_up.x;
	view(1, 1) = m_up.y;
	view(2, 1) = m_up.z;
	view(3, 1) = y;

	view(0, 2) = m_target.x;
	view(1, 2) = m_target.y;
	view(2, 2) = m_target.z;
	view(3, 2) = z;

	view(0, 3) = 0.0f;
	view(1, 3) = 0.0f;
	view(2, 3) = 0.0f;
	view(3, 3) = 1.0f;
}

void FreeCamera::CreateProjectionMatrix(Matrix &projection)
{
	MatrixPerspectiveFovLH(&projection, GetFieldofView(), GetAspectRatio(), GetNearPlane(), GetFarPlane());
}
