#include "Vertex.h"
#include "Material.h"
#include "Quad.h"
#include "Error.h"

Quad::Quad() 
	:
	m_numSubDivisions(0),
	m_vertexBuffer(0),
	m_numberOfVerts(0)
{

}

Quad::Quad(const Vector3D& position, const Vector3D& rotation, int width, 
	int height, int numSubDivisions, DWORD colour)
	:
	m_numSubDivisions(0),
	m_vertexBuffer(0),
	m_numberOfVerts(0)
{
	CreateQuad(position, rotation, width, height, numSubDivisions, colour);
}

Quad::~Quad()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Quad::CreateQuad(const Vector3D& position, const Vector3D& rotation, int width, int height, int numSubDivisions, DWORD colour)
{
	m_position = position;
	m_rotation = rotation;
	m_width = width;
	m_height = height;
	m_colour = colour;

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (numSubDivisions <= 0)
	{
		ErrorMessage("Cannot create a quad with no sub divisions");
	}

	m_numSubDivisions = numSubDivisions;
	m_numberOfVerts = numSubDivisions * numSubDivisions * 6;

	const float delta = 1.0f / ((float)m_numSubDivisions);

	m_D3DDevice->CreateVertexBuffer(m_numberOfVerts * sizeof(VertexPositionNormalColour), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertexBuffer, 0);

	VertexPositionNormalColour* vertex;
	Vector3D normal(0, 0, -1);

	m_vertexBuffer->Lock(0, 0, (void**)&vertex, 0);

	int counter = 0;
	for (int y = 0; y < numSubDivisions; y++)
	{
		for (int x = 0; x < numSubDivisions; x++)
		{
			Vector3D p0((width * x * delta) - width / 2, (height * y * delta) - height / 2, 0);
			Vector3D p1((width * x * delta) - width / 2, (height * (y + 1)* delta) - height / 2, 0);
			Vector3D p2((width * (x + 1) * delta) - width / 2, (height * (y + 1) * delta) - height / 2, 0);
			Vector3D p3((width * (x + 1) * delta) - width / 2, (height * y * delta) - height / 2, 0);

			//create the first poly
			vertex[counter + 0] = VertexPositionNormalColour(p0, normal, m_colour);
			vertex[counter + 1] = VertexPositionNormalColour(p1, normal, m_colour);
			vertex[counter + 2] = VertexPositionNormalColour(p2, normal, m_colour);
			vertex[counter + 3] = VertexPositionNormalColour(p0, normal, m_colour);
			vertex[counter + 4] = VertexPositionNormalColour(p2, normal, m_colour);
			vertex[counter + 5] = VertexPositionNormalColour(p3, normal, m_colour);

			counter += 6;
		}
	}

	m_vertexBuffer->Unlock();
}

void Quad::SetPosition(const Vector3D& position)
{
	m_position = position;
}

void Quad::SetRotation(const Vector3D& rotation)
{
	m_rotation = rotation;
}

void Quad::SetWidth(int width)
{
	CreateQuad(m_position, m_rotation, width, m_height, m_numSubDivisions, m_colour);
}

void Quad::SetHeight(int height)
{
	CreateQuad(m_position, m_rotation, m_width, height, m_numSubDivisions, m_colour);
}

void Quad::SetNumSubDivisions(int numSubDivisions)
{
	m_vertexBuffer->Release();
	CreateQuad(m_position, m_rotation, m_width, m_height, numSubDivisions, m_colour);
}

void Quad::Render()
{
	Matrix positionMatrix;
	Matrix rotationMatrix;
	Matrix world;

	//create matrix to move quad in world;
	MatrixTranslation(&positionMatrix, m_position.x, m_position.y, m_position.z);
	MatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(m_rotation.y), D3DXToRadian(m_rotation.x), D3DXToRadian(m_rotation.z));
	world = rotationMatrix * positionMatrix;

	m_D3DDevice->SetTransform(D3DTS_WORLD, &world);

	m_D3DDevice->SetStreamSource(0, m_vertexBuffer, 0, sizeof(VertexPositionNormalColour));

	m_D3DDevice->SetVertexDeclaration(VertexPositionNormalColour::Decl);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	m_D3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_numberOfVerts / 3);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
