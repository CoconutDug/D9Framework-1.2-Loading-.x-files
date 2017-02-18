#include "Types.h"
#include "Material.h"
#include "CameraController.h"
#include "Sphere.h"

int	Sphere::ms_numSpheres = 0;
LPD3DXMESH Sphere::ms_sphereMesh = 0;

Sphere::Sphere() 
	:
	m_position(Vector3D(0, 0, 0)),
	m_radius(1.0f),
	m_colour(0xffffffff)
{
	ms_numSpheres++;
}

Sphere::Sphere(const AABB &aabb) 
	:
	m_colour(0xffffffff)
{
	m_position = (aabb.GetMin() + aabb.GetMax()) * 0.5f;
	m_radius = Vector3DLength(&(aabb.GetMax() - m_position));
	ms_numSpheres++;
}

Sphere::Sphere(const Vector3D& position, float radius) 
	:
	m_position(position),
	m_radius(radius),
	m_colour(0xffffffff)
{
	ms_numSpheres++;
}

Sphere::Sphere(const Vector3D& position, float radius, DWORD colour) :
	m_position(position),
	m_radius(radius),
	m_colour(colour)
{
	ms_numSpheres++;
}

Sphere::Sphere(const Sphere& other) 
	:
	m_position(other.m_position),
	m_radius(other.m_radius),
	m_colour(other.m_colour)
{
	ms_numSpheres++;
}

Sphere::~Sphere()
{
	ms_numSpheres--;
	if (ms_sphereMesh && !ms_numSpheres)
	{
		ms_sphereMesh->Release();
		ms_sphereMesh = 0;
	}
}

bool Sphere::CollidingWithSphere(const Sphere& sphere)
{
	Vector3D vecTo = sphere.GetPosition() - m_position;
	float distance = Vector3DLengthSq(&vecTo);
	float radiusSum = sphere.GetRadius() + m_radius;
	if (distance >  radiusSum  * radiusSum)
	{
		return false;
	}
	return true;
}

void Sphere::Render(bool wireframe)
{
	if (!ms_sphereMesh)
	{
		CreateWireFrameSphere();
	}

	Matrix world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, m_position.x, m_position.y, m_position.z);

	/// Scale the transform to the size of the sphere
	world._11 = m_radius;
	world._22 = m_radius;
	world._33 = m_radius;

	Material mat(m_colour, m_colour, D3DCOLOR_XRGB(0, 0, 0), 0.f);
	mat.m_emissive = mat.m_ambient;

	m_D3DDevice->SetVertexShader(NULL);
	m_D3DDevice->SetPixelShader(NULL);
	CameraController::GetInstance()->SetWorld(world);
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_D3DDevice->SetMaterial(mat);
	ms_sphereMesh->DrawSubset(0);

	CameraController::GetInstance()->ResetWorld();
}

void Sphere::SetRadius(float radius)
{
	m_radius = radius;
}

void Sphere::CreateWireFrameSphere()
{
	if (!m_D3DDevice)
	{
		return;
	}

	D3DXCreateSphere(m_D3DDevice, 1.f, 20, 20, &ms_sphereMesh, NULL);
}