#pragma once
#include "AABB.h"

class Sphere;

class Frustum
{
public:
	Frustum();
	~Frustum();
	void Calculate(const Matrix& view, const Matrix& projection, bool normalize = true);
	bool PointVisible(const Vector3D& point) const;
	bool SphereVisible(const Sphere& sphere) const;
	bool SphereVisible(const Vector3D& position, float radius) const;
	bool BoxVisible(const AABB& aabb) const;
private:
	enum
	{
		PLEFT = 0,
		PRIGHT,
		PTOP,
		PBOTTOM,
		PNEAR,
		PFAR,
		NUM_PLANES
	};

	struct Plane
	{
		Vector3D normal;
		float distance;
		inline void Normalise()
		{
			float denom = 1 / Vector3DLength(&normal);
			normal *= denom;
			distance *= denom;
		}
	};
	float DistancePointPlane(const Plane& testPlane, const Vector3D& point) const;
	Plane m_frustumPlanes[NUM_PLANES];
};