#include "Model.h"
#include "CameraController.h"
#include "Vertex.h"
#include "Error.h"

Model::Model(void)
{

}

Model::Model(LPCSTR fileName)
{
	LoadModel(fileName);
	m_position = Vector3D(0, 0, 0);
	m_rotation = Vector3D(0, 0, 0);
	m_scale = 1.0f;

}

Model::Model(LPCSTR fileName, Vector3D& position, Vector3D& rotation, float scale)
{
	LoadModel(fileName);
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Model::~Model(void)
{
	m_mesh->Release();
	
	for (int i = 0; i < (int)m_textures.size(); ++i)
		if (m_textures[i])
			m_textures[i]->Release();
}

//bool Model::LoadModel(const char *fileName)
//
//{
//
//	// Load the model into a system memory mesh
//
//	HRESULT      hr;
//
//	Mesh*         meshSys = NULL;
//
//	ID3DXBuffer* adjBuffer = NULL;
//
//	ID3DXBuffer* materialBuffer = NULL;
//
//	DWORD        numberMaterials = 0;
//
//
//
//	// load the x model into meshSys using system memory:
//
//	hr = D3DXLoadMeshFromXA(fileName, D3DXMESH_SYSTEMMEM, m_D3DDevice, &adjBuffer, &materialBuffer, 0, &numberMaterials, &meshSys);
//
//	if (hr != D3D_OK)
//
//		ErrorMessage("D3DXLoadMeshFromX failed");
//
//
//
//	// Get it's vertex buffer layout into ModelDecl:
//
//	D3DVERTEXELEMENT9 ModelDecl[MAX_FVF_DECL_SIZE];
//
//	hr = meshSys->GetDeclaration(ModelDecl);
//
//	if (hr != D3D_OK)
//
//		ErrorMessage("ID3DXBaseMesh::GetDeclaration failed");
//
//
//
//	// Clone the mesh into m_pMesh, with ModelDecl specifying the vertex format for the output mesh
//
//	hr = meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, ModelDecl, m_D3DDevice, &m_mesh); // Cloned mesh m_mesh
//
//	if (hr != D3D_OK) // is now associatied with g_pD3D
//
//		ErrorMessage("ID3DXBaseMesh::CloneMesh failed");
//
//
//
//	// Get the new cloned mesh's vertex format
//
//	D3DVERTEXELEMENT9 NewDecl[MAX_FVF_DECL_SIZE];
//
//	hr = m_mesh->GetDeclaration(NewDecl);
//
//	if (hr != D3D_OK)
//
//		ErrorMessage("ID3DXBaseMesh::GetDeclaration failed");
//
//
//
//
//
//	// Check if the original mesh model contains normals and tangents:
//
//	bool bHadNormal = false;
//
//	bool bHadTangent = false;
//
//	for (unsigned int index = 0; index < D3DXGetDeclLength(ModelDecl); ++index)
//
//	{
//
//		if (ModelDecl[index].Usage == D3DDECLUSAGE_NORMAL)
//
//			bHadNormal = true;
//
//
//
//		if (ModelDecl[index].Usage == D3DDECLUSAGE_TANGENT)
//
//			bHadTangent = true;
//
//	}
//
//
//
//	// Check if the cloned mesh contains a normals and tangents:
//
//	bool bHaveNormalNow = false;
//
//	bool bHaveTangentNow = false;
//
//	for (unsigned int index = 0; index < D3DXGetDeclLength(NewDecl); ++index)
//
//	{
//
//		if (NewDecl[index].Usage == D3DDECLUSAGE_NORMAL)
//
//			bHaveNormalNow = true;
//
//
//
//		if (NewDecl[index].Usage == D3DDECLUSAGE_TANGENT)
//
//			bHaveTangentNow = true;
//
//	}
//
//
//
//	// We can now release the original meshSys:
//
//	meshSys->Release();
//
//
//
//	// Now work on m_mesh to compute normals and tangents if necessary:
//
//	if (!bHadNormal && bHaveNormalNow)
//
//		D3DXComputeNormals(m_mesh, NULL);  // Compute normals in case the meshes have them
//
//
//
//
//
//	if (bHaveNormalNow && !bHadTangent && bHaveTangentNow)
//
//	{ // Need to compute tangents
//
//		Mesh* newMeshSys;
//
//
//
//		DWORD* rgdwAdjacency = NULL;
//
//		rgdwAdjacency = new DWORD[m_mesh->GetNumFaces() * 3];
//
//		if (rgdwAdjacency == NULL)
//
//			ErrorMessage("Out of memory, mesh too big");
//
//
//
//		hr = m_mesh->GenerateAdjacency(1e-6f, rgdwAdjacency);
//
//		if (hr != D3D_OK)
//
//			ErrorMessage("ID3DXBaseMesh::GenerateAdjacency failed");
//
//
//
//		// Compute tangents, which we need for normal mapping
//
//		hr = D3DXComputeTangentFrameEx(m_mesh,
//
//			D3DDECLUSAGE_TEXCOORD, 0,
//
//			D3DDECLUSAGE_TANGENT, 0,
//
//			D3DX_DEFAULT, 0,
//
//			D3DDECLUSAGE_NORMAL, 0,
//
//			0, rgdwAdjacency,
//
//			0.01f, 0.25f, 0.01f, // fPartialEdgeThreshold, fSingularPointThreshold, fNormalEdgeThreshold,
//
//			&newMeshSys, NULL);
//
//
//
//		delete[] rgdwAdjacency;
//
//		if (hr != D3D_OK)
//
//			ErrorMessage("D3DXComputeTangentFrameEx failed");
//
//
//
//		m_mesh->Release();
//
//
//
//		m_mesh = newMeshSys;
//
//	}
//
//
//
//	// The model is ready, now we need to load the textures and materials - Texture information for the model is
//
//	// stored within a stack, it can be stored as using either an absolute or a relative texture path, depending
//
//	// on the options when the model was created.
//
//	//
//
//	if (materialBuffer != NULL && numberMaterials != 0)
//
//	{
//
//		D3DXMATERIAL* d3dxmaterial = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();
//
//
//
//		for (DWORD i = 0; i < numberMaterials; ++i)
//
//		{
//
//			// Note: the MatD3D property does not have an ambient
//
//			// value set when its loaded, so just set it to the diffuse value.
//
//			Material* m = new Material;
//
//			m->m_ambient = d3dxmaterial[i].MatD3D.Diffuse;
//
//			m->m_diffuse = d3dxmaterial[i].MatD3D.Diffuse;
//
//			m->m_specular = d3dxmaterial[i].MatD3D.Specular;
//
//			m->m_specularPower = d3dxmaterial[i].MatD3D.Power;
//
//
//
//			m_materials.push_back(m);
//
//
//			// Check if this material has a external texture
//
//
//
//			if (d3dxmaterial[i].pTextureFilename != NULL)
//
//			{
//
//				MyString tempString;
//
//				tempString.Format("%s%s%s", Program::GetInstance()->GetDirectory(), ASSETBASETEXTUREPATH, d3dxmaterial[i].pTextureFilename);
//
//
//
//				int textureIndex = TextureCache::GetInstance()->AddTexture(tempString, tempString, true);
//
//				m_textureIndexList.push_back(textureIndex);
//
//			}
//
//			else
//
//				m_textureIndexList.push_back(-1);
//
//		}
//
//	}
//
//	materialBuffer->Release();
//
//	//model is ready
//
//	return 1;
//
//}

bool Model::LoadModel(LPCSTR fileName)
{	
	Mesh* meshSys = 0;
	ID3DXBuffer* adjBuffer = 0;
	ID3DXBuffer* materialBuffer = 0;
	DWORD numberMaterials = 0;

	D3DXLoadMeshFromXA(fileName, D3DXMESH_SYSTEMMEM, m_D3DDevice, &adjBuffer, &materialBuffer, 0, &numberMaterials, &meshSys);

	//The Model is loaded, now, check to see if the model contains 
	//normal data, this will be used later on for effects
	//if they don't d3d can create them
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	meshSys->GetDeclaration(elems);

	bool modelHasNormals = false;

	D3DVERTEXELEMENT9 term = D3DDECL_END();
	for (int i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{
		if (elems[i].Stream == 0xff)
		{
			break;
		}

		if (elems[i].Type == D3DDECLTYPE_FLOAT3 && elems[i].Usage == D3DDECLUSAGE_NORMAL && elems[i].UsageIndex == 0)
		{
			modelHasNormals = true;
			break;
		}
	}

	//The Model will now be converted to support a created vertex type,
	//in this case, the VertexPositionNormalTexCoord type

	D3DVERTEXELEMENT9 elements[64];
	UINT numberElements = 0;
	VertexPositionNormalTexCoord::Decl->GetDeclaration(elements, &numberElements);

	Mesh* temp = 0;
	meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, m_D3DDevice, &temp);
	meshSys->Release();
	meshSys = temp;

	//With the Mesh Copied and converted to the VertexPositionNormalTexCoord type
	//if the model didn't contain normal data, we will create it, as it can now be
	//stored within the VertexPositionNormalTexCoord type

	if (!modelHasNormals)
	{
		D3DXComputeNormals(meshSys, 0);
	}

	//we now optimise the mesh and store it in the final mesh variable
	meshSys->Optimize(D3DXMESH_MANAGED | D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0, &m_mesh);
	meshSys->Release();
	adjBuffer->Release();

	//The model is ready, now we need to load the textures and materials
	//(if any)
	if (materialBuffer != 0 && numberMaterials != 0)
	{
		D3DXMATERIAL* d3dxmaterial = (D3DXMATERIAL*)materialBuffer->GetBufferPointer();

		for (DWORD i = 0; i < numberMaterials; i++)
		{
			// Note: the MatD3D property does not have an ambient
			// value set when its loaded, so just set it to the diffuse value.
			Material m;
			m.m_ambient = d3dxmaterial[i].MatD3D.Diffuse;
			m.m_diffuse = d3dxmaterial[i].MatD3D.Diffuse;
			m.m_specular = d3dxmaterial[i].MatD3D.Specular;
			m.m_specularPower = d3dxmaterial[i].MatD3D.Power;
			m_materials.push_back(m);

			//check if this material has a external texture;

			if (d3dxmaterial[i].pTextureFilename != 0)
			{
				IDirect3DTexture9* tex = 0;
				LPCSTR textureFileName = d3dxmaterial[i].pTextureFilename;
				D3DXCreateTextureFromFileA(m_D3DDevice, textureFileName, &tex);
				m_textures.push_back(tex);
			}
			else
			{
				m_textures.push_back(0);
			}

		}
	}
	materialBuffer->Release();

	//model is ready
	return true;
}

void Model::Render()
{
	if (m_mesh)
	{

		Matrix world, scale, position;
		Matrix rotateX, rotateY, rotateZ;

		MatrixScaling(&scale, m_scale, m_scale, m_scale);

		MatrixRotationY(&rotateY, m_rotation.y);
		MatrixRotationX(&rotateX, m_rotation.x);
		MatrixRotationZ(&rotateZ, m_rotation.z);

		MatrixTranslation(&position, m_position.x, m_position.y, m_position.z);

		MatrixMultiply(&world, &scale, &rotateY);
		MatrixMultiply(&world, &world, &rotateX);
		MatrixMultiply(&world, &world, &rotateZ);
		MatrixMultiply(&world, &world, &position);

		CameraController::GetInstance()->SetWorld(world);

		for (int j = 0; j < (int)m_materials.size(); j++)
		{
			m_D3DDevice->SetMaterial(m_materials[j]);
			m_D3DDevice->SetTexture(j, m_textures[j]);
			m_mesh->DrawSubset(j);
		}
		m_D3DDevice->SetTexture(0, NULL);

		CameraController::GetInstance()->ResetWorld();
	}
}

void Model::SetPosition(Vector3D& position)
{
	SetPosition(position.x,position.y,position.z);
}


void Model::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


Vector3D Model::GetPosition() const
{
	return m_position;
}


void Model::SetRotation(Vector3D rotation)
{
	SetRotation(rotation.x,rotation.y,rotation.z);
}

void Model::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


void Model::SetScale(float scale)
{
	m_scale = scale;
}

float Model::GetScale()
{
	return m_scale;
}
