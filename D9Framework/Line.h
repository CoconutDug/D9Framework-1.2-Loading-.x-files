#pragma once
#include "Types.h"

class Line
{
public:
	Line(const Vector3D& from = Vector3D(0, 0, 0), const Vector3D& to = Vector3D(0, 0, 0), 
		DWORD fromColour = 0x00000000, DWORD toColour = 0x00000000);
	~Line();
	void Setup(const Vector3D& from, const Vector3D& to, DWORD fromColour = 0x00000000, DWORD toColour = 0x00000000);
	void Render();
	void SetPosition(const Vector3D& from, const Vector3D& to);
private:
	void CreateLine();

	Vector3D m_pointA;
	Vector3D m_pointB;
	DWORD m_pointAColour;
	DWORD m_pointBColour;
	IDirect3DVertexBuffer9*	m_vertexBuffer;
};
