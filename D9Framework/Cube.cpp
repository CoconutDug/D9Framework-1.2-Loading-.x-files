#include "Vertex.h"
#include "Cube.h"
#include "InputManager.h"

Cube::Cube()
{
	BuildVertexBuffer();
	BuildIndexBuffer();
	m_wireFrame = false;
}

Cube::~Cube()
{
	m_vertexBuffer->Release();
	m_triangleListIndexBuffer->Release();
}

void Cube::Render()
{
	/// Set the vertex buffer to use
	m_D3DDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VertexColour));

	/// Set the index buffer to use
	m_D3DDevice->SetIndices(m_triangleListIndexBuffer);

	/// Set the vertex declaration, to tell d3d how our vertices are made up
	m_D3DDevice->SetVertexDeclaration(VertexColour::Decl);

	/// Turns off lighting
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_wireFrame)
	{
		m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	/// Render the geomtry as a triangle list, with 8 verts and 12 indices
	m_D3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	/// Re-enable lighting
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Cube::Update(float elapsedTime)
{
	/// Check to see if we need to toggle the wireframe mode
	if (InputManager::GetInstance()->IsKeyClicked(true, '1'))
	{
		m_wireFrame = !m_wireFrame;
	}
}

void Cube::BuildVertexBuffer()
{
	m_D3DDevice->CreateVertexBuffer(8 * sizeof(VertexColour), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertexBuffer, 0);

	VertexColour* vertex = 0;

	m_vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

	vertex[0] = VertexColour(-1.0f, 1.0f, -1.0f, 0x00000000);
	vertex[1] = VertexColour(1.0f, 1.0f, -1.0f, 0x000000ff);
	vertex[2] = VertexColour(-1.0f, -1.0f, -1.0f, 0x0000ff00);
	vertex[3] = VertexColour(1.0f, -1.0f, -1.0f, 0x0000ffff);

	vertex[4] = VertexColour(-1.0f, 1.0f, 1.0f, 0x00ffff00);
	vertex[5] = VertexColour(-1.0f, -1.0f, 1.0f, 0x00ff0000);
	vertex[6] = VertexColour(1.0f, 1.0f, 1.0f, 0x00ff00ff);
	vertex[7] = VertexColour(1.0f, -1.0f, 1.0f, 0x00ffffff);

	m_vertexBuffer->Unlock();
}

void Cube::BuildIndexBuffer()
{
	m_D3DDevice->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_triangleListIndexBuffer, 0);

	WORD* index;

	m_triangleListIndexBuffer->Lock(0, 0, (void**)&index, 0);

	//triangeList
	// Front Face
	index[0] = 2;
	index[1] = 0;
	index[2] = 1;
	index[3] = 2;
	index[4] = 1;
	index[5] = 3;

	// Back face.
	index[6] = 5;
	index[7] = 6;
	index[8] = 4;
	index[9] = 5;
	index[10] = 7;
	index[11] = 6;

	// Left face.
	index[12] = 5;
	index[13] = 4;
	index[14] = 0;
	index[15] = 5;
	index[16] = 0;
	index[17] = 2;

	// Right face.
	index[18] = 3;
	index[19] = 1;
	index[20] = 6;
	index[21] = 3;
	index[22] = 6;
	index[23] = 7;

	// Top face.
	index[24] = 0;
	index[25] = 4;
	index[26] = 6;
	index[27] = 0;
	index[28] = 6;
	index[29] = 1;

	// Bottom face.
	index[30] = 5;
	index[31] = 2;
	index[32] = 3;
	index[33] = 5;
	index[34] = 3;
	index[35] = 7;

	m_triangleListIndexBuffer->Unlock();
}

/************************************************************************/
