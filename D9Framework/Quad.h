#pragma once
#include "Types.h"

class Material;

class Quad
{
public:
	Quad();
	Quad(const Vector3D& position, const Vector3D& rotation, int width, 
		int height, int numSubDivisions, DWORD colour);
	~Quad();
	void CreateQuad(const Vector3D& position, const Vector3D& rotation, int width,
		int height, int numSubDivisions, DWORD colour);
	void Render();
	void SetWidth(int width);
	void SetHeight(int height);
	void SetNumSubDivisions(int numSubDivisions);
	void SetPosition(const Vector3D& position);
	void SetRotation(const Vector3D& rotation);
private:
	Vector3D m_position;
	Vector3D m_rotation;
	int m_width;
	int m_height;
	int m_numSubDivisions;
	int m_numberOfVerts;
	DWORD m_colour;
	IDirect3DVertexBuffer9* m_vertexBuffer;
};