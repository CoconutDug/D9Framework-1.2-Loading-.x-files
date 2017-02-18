#pragma once

/*
The D3DXFRAME structure represents a BONE in the skeleton hierarchy.  It contains the bone transformation matrix
and pointers to child and sibling bones as well as a name for the bone and a container that may or may not contain
geometry attached to the bone.
Here we are inheriting form D3DXFRAME and adding our own combined matrix which will be used when animating the hierarchy.
*/

struct Frame : public D3DXFRAME
{
	Matrix  combinedTransform;
};

/*
D3DXMESHCONTAINER contains the geometry that will be attached to the bones.  In other words the
SKIN the process of attaching this geometry is known as Skinning.
Here we have derived from the D3DXMESHCONTAINER to add our own data that will simplyfy loading and
skinning the model.
*/
struct MeshContainer : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* m_textures;
	D3DMATERIAL9* m_material;

	Mesh* m_mesh;
	Matrix * m_boneOffset;
	Matrix ** m_frameMatrix;
};


/*
ID3DXAllocateHierarchy is an interface provided by DirectX that is used to allocate and free
memory for loading our mesh hierarchy. Because the  D3DXFRAME and D3DXMESHCONTAINER are usually
derived from and extended it is up to the programmer to implement the functionality of this class.
We must provide the implementation of the interface	ourselves by inheriting form it and providing
code for the CreateFrame, CreateMeshContainer,	DestroyFrame and DestroyMeshContainer functions.
*/
class Hierarchy : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(LPCSTR name, LPD3DXFRAME *newFrame);
	STDMETHOD(CreateMeshContainer) (LPCSTR name, const D3DXMESHDATA* meshData, const D3DXMATERIAL* materials, const D3DXEFFECTINSTANCE* effects,
		DWORD numMaterials, const DWORD* adjacency, LPD3DXSKININFO skinInfo, LPD3DXMESHCONTAINER *newMeshContainer);
	STDMETHOD(DestroyFrame)(LPD3DXFRAME frame);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER meshContainerToFree);
};