#pragma once
#include "Vertex.h"
#include "AABB.h"

class Material;

class Sphere
{
public:
	Sphere();
	Sphere(const AABB& aabb);
	Sphere(const Vector3D& position, float radius);
	Sphere(const Vector3D& position, float radius, DWORD colour);
	Sphere(const Sphere& other);
	~Sphere();
	void SetPosition(const Vector3D& position) 
	{
		m_position = position;
	}
	const Vector3D&	GetPosition()const { return m_position; }
	void SetRadius(float radius);
	float GetRadius()const 
	{ 
		return m_radius; 
	}
	bool CollidingWithSphere(const Sphere& otherSphere);
	void Render(bool wireframe = true);
private:
	static void	CreateWireFrameSphere();
	Vector3D m_position;
	float m_radius;
	DWORD m_colour;
	static LPD3DXMESH ms_sphereMesh;
	static int ms_numSpheres;
};
