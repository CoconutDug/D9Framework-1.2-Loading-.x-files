#pragma once
#include "Types.h"
#include <float.h>

class PolygonData;

class AABB
{
public:
	AABB();
	AABB(const Vector3D& min, const Vector3D& max);
	AABB(const Vector3D* vertArray, int vertCount);
	AABB(Mesh* mesh);
	~AABB();
	void SetMin(const Vector3D& min);
	const Vector3D& GetMin() const;
	void SetMax(const Vector3D& max);
	const Vector3D& GetMax() const;
	void MoveAABB(const Vector3D& position);
	bool CheckCollisionAABB(const AABB& otherAABB) const;
	bool SurroundsAABB(const AABB& otherAABB) const;
	void AddPoint(const Vector3D& pos);

private:
	Vector3D m_min;
	Vector3D m_max;
};