#include "Vertex.h"

IDirect3DVertexDeclaration9* VertexPosition::Decl = 0;
IDirect3DVertexDeclaration9* VertexColour::Decl = 0;
IDirect3DVertexDeclaration9* VertexPositionTexCoord::Decl = 0;
IDirect3DVertexDeclaration9* VertexPositionNormalColour::Decl = 0;
IDirect3DVertexDeclaration9* VertexPositionNormalTexCoord::Decl = 0;
IDirect3DVertexDeclaration9* BillBoardVertex::Decl = 0;

void InitAllVertexDeclarations()
{
	// VertexPosition
	//create an array of D3DVERTEXELEMENT9 containing the information required for each piece of 
	//vertex information
	D3DVERTEXELEMENT9 VertexPositionElements[] =
	{
		//Each D3DVERTEXELEMENT9 is defines below.
		//{Stream number, offset to the beginning of this piece of vertex information, variable type,
		//Method for tessellator processing, the usage for this Element, the usage index to specify multiple usage types }
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	//VertexPosition is then declared to the device
	m_D3DDevice->CreateVertexDeclaration(VertexPositionElements, &VertexPosition::Decl);

	// VertexColour
	D3DVERTEXELEMENT9 VertexColourElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	m_D3DDevice->CreateVertexDeclaration(VertexColourElements, &VertexColour::Decl);

	// VertexPositionTexCoord
	D3DVERTEXELEMENT9 VertexPositionTexCoordElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	m_D3DDevice->CreateVertexDeclaration(VertexPositionTexCoordElements, &VertexPositionTexCoord::Decl);

	// VertexPositionNormalColour
	D3DVERTEXELEMENT9 VertexPositionNormalColourElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};
	m_D3DDevice->CreateVertexDeclaration(VertexPositionNormalColourElements, &VertexPositionNormalColour::Decl);

	// VertexPositionNormalTexCoord
	D3DVERTEXELEMENT9 VertexPositionNormalTexCoordElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	m_D3DDevice->CreateVertexDeclaration(VertexPositionNormalTexCoordElements, &VertexPositionNormalTexCoord::Decl);

	D3DVERTEXELEMENT9 BillBoardVertexElements[] =
	{
		{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		D3DDECL_END()
	};
	m_D3DDevice->CreateVertexDeclaration(BillBoardVertexElements, &BillBoardVertex::Decl);

}

void DestroyAllVertexDeclarations()
{
	VertexPosition::Decl->Release();
	VertexColour::Decl->Release();
	VertexPositionTexCoord::Decl->Release();
	VertexPositionNormalColour::Decl->Release();
	VertexPositionNormalTexCoord::Decl->Release();
	BillBoardVertex::Decl->Release();
}
