#include "Types.h"
#include "ThirdPersonCamera.h"
#include "Program.h"
#include "InputManager.h"

const float TIMETORESETCAMERA = 0.25f;

ThirdPersonCamera::ThirdPersonCamera()
{
	m_distance = 9.2f;
	m_targetDistance = m_distance;
	m_position = Vector3D(0, 0, 0);
	m_yaw = 0.0f;
	m_targetYaw = m_yaw;
	m_pitch = 0.0f;
	m_targetPitch = m_pitch;
	m_tendTo = false;
	m_intersectionTime = 1.0f;
}

ThirdPersonCamera::~ThirdPersonCamera()
{

}

void ThirdPersonCamera::CreateViewMatrix(Matrix &view)
{
	float x = m_target.x + m_distance * sinf(m_yaw) * cosf(m_pitch);
	float z = m_target.z + m_distance * cosf(m_yaw) * cosf(m_pitch);
	m_position.x = x;
	m_position.y = m_target.y + m_distance * sin(m_pitch);
	m_position.z = z;

	Vector3D intersectionPoint, startPoint;
	startPoint = m_target;
	startPoint.y += 2.5f;

	m_position = m_position * m_intersectionTime + startPoint * (1 - m_intersectionTime);

	m_up = Vector3D(0, 1, 0);
	Matrix rotate;
	MatrixRotationX(&rotate, 0.3f - m_distance * m_intersectionTime / 150.0f);
	MatrixLookAtLH(&view, &m_position, &m_target, &m_up);
	view = view * rotate;
}

void ThirdPersonCamera::CreateProjectionMatrix(Matrix &projection)
{
	MatrixPerspectiveFovLH(&projection, GetFieldofView(), GetAspectRatio(), GetNearPlane(), GetFarPlane());
}

void ThirdPersonCamera::ProcessKeys(float elapsedTime)
{
	//moving the mouse left and right will rotate the camera around the target
	if (InputManager::GetInstance()->IsMouseButtonDown(1))
	{
		m_yaw += InputManager::GetInstance()->GetMouseX() / 100.0f;
		m_pitch += InputManager::GetInstance()->GetMouseY() / 100.0f;

		m_pitch = clamp(m_pitch, 0.1f * D3DX_PI / 2, 0.9f * D3DX_PI / 2);

		m_tendTo = false;
	}
	else if (m_tendTo)
	{
		if (m_targetYaw - m_yaw > D3DX_PI)
		{
			m_yaw += 2 * D3DX_PI;
		}
		else if (m_targetYaw - m_yaw < -D3DX_PI)
		{
			m_yaw -= 2 * D3DX_PI;
		}

		m_yaw = lerp(m_yaw, m_targetYaw, clamp(elapsedTime * 8, 0.0f, 1.0f));

		if (m_targetPitch - m_pitch > D3DX_PI)
		{
			m_pitch += 2 * D3DX_PI;
		}
		else if (m_targetPitch - m_pitch < -D3DX_PI)
		{
			m_pitch -= 2 * D3DX_PI;
		}

		m_pitch = lerp(m_pitch, m_targetPitch, clamp(elapsedTime * 8, 0.0f, 1.0f));
	}

	//GetMouseZ()is the mouse wheel, this will zoom into the object
	m_targetDistance -= InputManager::GetInstance()->GetMouseZ() / 64.0f;

	if (m_targetDistance < 4.4f)
	{
		m_targetDistance = 4.4f;
	}
	else if (m_targetDistance > 20.0f)
	{
		m_targetDistance = 20.0f;
	}

	m_distance = lerp(m_distance, m_targetDistance, clamp(elapsedTime * 8, 0.0f, 1.0f));

	m_intersectionTime = lerp(m_intersectionTime, 1.0f, clamp(elapsedTime * 4, 0.0f, 1.0f));
}

void ThirdPersonCamera::SetDistanceFromObject(float distance)
{
	m_distance = distance;
	m_targetDistance = distance;
}

void ThirdPersonCamera::SetUpThirdPersonCamera(float yaw, float pitch)
{
	m_yaw = yaw;
	m_targetYaw = yaw;
	m_pitch = pitch;
	m_targetPitch = pitch;
}

void ThirdPersonCamera::SetTargetYaw(float yaw)
{
	m_targetYaw = yaw;
	m_tendTo = true;
}
