#include "Line.h"
#include "CameraController.h"
#include "Vertex.h"

Line::Line(const Vector3D& from, const Vector3D& to, DWORD fromColour, DWORD toColour) :
	m_vertexBuffer(0)
{
	Setup(from, to, fromColour, toColour);
}

Line::~Line()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
	}
}

void Line::CreateLine()
{
	if (!m_vertexBuffer)
	{
		m_D3DDevice->CreateVertexBuffer(2 * sizeof(VertexColour), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertexBuffer, 0);
	}

	VertexColour* vertex = 0;

	m_vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

	vertex[0] = VertexColour(m_pointA, m_pointAColour);
	vertex[1] = VertexColour(m_pointB, m_pointBColour);

	m_vertexBuffer->Unlock();
}

void Line::Render()
{
	/// Revert the D3D device to using FF pipeline
	m_D3DDevice->SetVertexShader(NULL);
	m_D3DDevice->SetPixelShader(NULL);

	Matrix world;
	D3DXMatrixIdentity(&world);
	CameraController::GetInstance()->SetWorld(world);
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_D3DDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VertexColour));
	m_D3DDevice->SetVertexDeclaration(VertexColour::Decl);
	m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_D3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Line::Setup(const Vector3D &from, const Vector3D &to, DWORD fromColour, DWORD toColour)
{
	m_pointA = from;
	m_pointB = to;
	m_pointAColour = fromColour;
	m_pointBColour = toColour;
	CreateLine();
}

void Line::SetPosition(const Vector3D &from, const Vector3D &to)
{
	m_pointA = from;
	m_pointB = to;
	CreateLine();
}

