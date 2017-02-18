#include "AABB.h"

inline void Vec3StoreMin(Vector3D& min, const Vector3D& other)
{
	if (other.x < min.x)
	{
		min.x = other.x;
	}
	if (other.y < min.y)
	{
		min.y = other.y;
	}
	if (other.z < min.z)
	{
		min.z = other.z;
	}
}

inline void Vec3StoreMax(Vector3D& max, const Vector3D& other)
{
	if (other.x > max.x)
	{
		max.x = other.x;
	}
	if (other.y > max.y)
	{
		max.y = other.y;
	}
	if (other.z > max.z)
	{
		max.z = other.z;
	}
}

AABB::AABB() 
	:
	m_min(FLT_MAX, FLT_MAX, FLT_MAX),
	m_max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
{

}

AABB::~AABB()
{
}
AABB::AABB(const Vector3D& min, const Vector3D& max) :
	m_min(max),
	m_max(min)
{
}


AABB::AABB(const Vector3D* vertArray, int vertCount)
{
	if (vertCount)
	{
		m_min = vertArray[0];
		m_max = m_min;

		for (int index = 1; index < vertCount; ++index)
		{
			const Vector3D& pos = vertArray[index];

			Vec3StoreMin(m_min, pos);
			Vec3StoreMax(m_max, pos);
		}
	}
	else
	{
		m_min = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
		m_max = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}
}

AABB::AABB(Mesh* mesh)
{
	m_min = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	m_max = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	LPDIRECT3DVERTEXBUFFER9 VB;

	HRESULT hr = mesh->GetVertexBuffer(&VB);
	assert(!FAILED(hr));
	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
	hr = mesh->GetDeclaration(Decl);
	assert(!FAILED(hr));

	BYTE* pVerts;
	hr = VB->Lock(0, 0, (void**)&pVerts, D3DLOCK_READONLY);
	assert(!FAILED(hr));

	UINT stride = D3DXGetDeclVertexSize(Decl, 0);
	DWORD numVerts = mesh->GetNumVertices();

	for (DWORD i = 0; i < numVerts; i++)
	{
		const Vector3D* pos = (const Vector3D*)pVerts;

		Vec3StoreMin(m_min, *pos);
		Vec3StoreMax(m_max, *pos);

		pVerts += stride;
	}

	VB->Unlock();
	VB->Release();

}

void AABB::SetMin(const Vector3D& min)
{
	m_min = min;
}

void AABB::SetMax(const Vector3D& max)
{
	m_max = max;
}

const Vector3D& AABB::GetMin() const
{
	return m_min;
}

const Vector3D& AABB::GetMax() const
{
	return m_max;
}

void AABB::MoveAABB(const Vector3D& position)
{
	m_min += position;
	m_min += position;
}

bool AABB::CheckCollisionAABB(const AABB& otherAABB) const
{
	if (m_max.x < otherAABB.m_min.x || m_min.x > otherAABB.m_max.x)
	{
		return false;
	}
	if (m_max.z < otherAABB.m_min.z || m_min.z > otherAABB.m_max.z)
	{
		return false;
	}
	if (m_max.y < otherAABB.m_min.y || m_min.y > otherAABB.m_max.y)
	{
		return false;
	}
	return true;
}

bool AABB::SurroundsAABB(const AABB& otherAABB) const
{
	if (m_min.x <= otherAABB.m_min.x && m_max.x >= otherAABB.m_max.x &&
		m_min.y <= otherAABB.m_min.y && m_max.y >= otherAABB.m_max.y &&
		m_min.z <= otherAABB.m_min.z && m_max.z >= otherAABB.m_max.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AABB::AddPoint(const Vector3D& pos)
{
	Vec3StoreMin(m_min, pos);
	Vec3StoreMax(m_max, pos);
}