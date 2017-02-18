#include "Types.h"
#include "Sphere.h"
#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::Calculate(const Matrix& view, const Matrix& projection, bool normalize)
{
	Matrix vP = view * projection;

	m_frustumPlanes[PLEFT].normal = Vector3D(vP._14 + vP._11, vP._24 + vP._21, vP._34 + vP._31);
	m_frustumPlanes[PLEFT].distance = vP._44 + vP._41;

	m_frustumPlanes[PRIGHT].normal = Vector3D(vP._14 - vP._11, vP._24 - vP._21, vP._34 - vP._31);
	m_frustumPlanes[PRIGHT].distance = vP._44 - vP._41;

	m_frustumPlanes[PTOP].normal = Vector3D(vP._14 - vP._12, vP._24 - vP._22, vP._34 - vP._32);
	m_frustumPlanes[PTOP].distance = vP._44 - vP._42;

	m_frustumPlanes[PBOTTOM].normal = Vector3D(vP._14 + vP._12, vP._24 + vP._22, vP._34 + vP._32);
	m_frustumPlanes[PBOTTOM].distance = vP._44 + vP._42;

	m_frustumPlanes[PNEAR].normal = Vector3D(vP._13, vP._23, vP._33);
	m_frustumPlanes[PNEAR].distance = vP._43;

	m_frustumPlanes[PFAR].normal = Vector3D(vP._14 - vP._13, vP._24 - vP._23, vP._34 - vP._33);
	m_frustumPlanes[PFAR].distance = vP._44 - vP._43;

	if (normalize)
	{
		for (int i = 0; i < NUM_PLANES; ++i)
		{
			m_frustumPlanes[i].Normalise();
		}
	}
}

float Frustum::DistancePointPlane(const Plane& testPlane, const Vector3D& point) const
{
	return Vector3DDotProduct(&testPlane.normal, &point) + testPlane.distance;
}

bool Frustum::PointVisible(const Vector3D& point) const
{
	for (int i = 0; i < NUM_PLANES; ++i)
	{
		if (DistancePointPlane(m_frustumPlanes[i], point) < 0)
		{
			//point is outside the frustum
			return false;
		}
	}
	/*point is either on or inside the frustum*/
	return true;
}
bool Frustum::SphereVisible(const Sphere& sphere) const
{
	float distance = 0.0f;
	for (int i = 0; i < NUM_PLANES; ++i)
	{
		distance = DistancePointPlane(m_frustumPlanes[i], sphere.GetPosition());
		if (distance < -sphere.GetRadius())
		{
			//Sphere is completely outside
			return false;
		}
		else if (distance < sphere.GetRadius())
		{
			/*sphere is intersecting the plane some of our geometry could be visible
			so return true.*/
			return true;
		}
	}
	/*Sphere is inside all of the PLANES so object is visible*/
	return true;
}

bool Frustum::SphereVisible(const Vector3D& position, float radius) const
{
	float distance = 0.0f;
	for (int i = 0; i < NUM_PLANES; ++i)
	{
		distance = DistancePointPlane(m_frustumPlanes[i], position);
		if (distance < -radius)
		{
			//Sphere is completely outside
			return false;
		}
		else if (distance < radius)
		{
			/*sphere is intersecting the plane some of our geometry could be visible
			so return true.*/
			return true;
		}
	}
	/*Sphere is inside all of the PLANES so object is visible*/
	return true;
}

bool Frustum::BoxVisible(const AABB &aabb) const
{
	float distance = 0.0f;
	Vector3D min = aabb.GetMin();
	Vector3D max = aabb.GetMax();
	int numVertsInside = 0;
	for (int i = 0; i < NUM_PLANES; ++i)
	{
		numVertsInside = 0;
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(min.x, min.y, min.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(max.x, min.y, min.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(min.x, max.y, min.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(min.x, min.y, max.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(max.x, max.y, min.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(min.x, max.y, max.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(max.x, min.y, max.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}
		distance = DistancePointPlane(m_frustumPlanes[i], Vector3D(max.x, max.y, max.z));
		if (distance >= 0)
		{
			numVertsInside++;
		}

		if (numVertsInside <= 0)
		{
			return false;
		}

	}
	return true;
}