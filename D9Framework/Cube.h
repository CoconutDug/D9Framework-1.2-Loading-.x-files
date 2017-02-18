#pragma once
#include "Types.h"

class Cube
{
public:
	Cube();
	~Cube();
	void Render();
	void Update(float elapsedTime);
private:
	void BuildVertexBuffer();
	void BuildIndexBuffer();
	bool m_wireFrame;
	IDirect3DVertexBuffer9* m_vertexBuffer;
	IDirect3DIndexBuffer9* m_triangleListIndexBuffer;
};
