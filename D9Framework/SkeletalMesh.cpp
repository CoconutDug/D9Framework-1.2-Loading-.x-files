#include "Types.h"
#include "CameraController.h"
#include "Hierarchy.h"
#include "SkeletalMesh.h"
#include "Line.h"
#include "Error.h"

SkeletalMesh::SkeletalMesh() 
	:
	m_maxBones(0),
	m_numberOfAnims(0),
	m_boneMatrices(0),
	m_rootFrame(0),
	m_aniCon(0)
{
}

SkeletalMesh::SkeletalMesh(const char* fileName) :
	m_maxBones(0),
	m_numberOfAnims(0),
	m_boneMatrices(0),
	m_rootFrame(0),
	m_aniCon(0)
{
	Load(fileName);
}

SkeletalMesh::~SkeletalMesh()
{
	Hierarchy* tempMH = new Hierarchy;

	D3DXFrameDestroy(m_rootFrame, tempMH);

	delete tempMH;

	if (m_boneMatrices)
	{
		delete[] m_boneMatrices;
		m_boneMatrices = 0;
	}

	if (m_aniCon)
	{
		m_aniCon->Release();
		m_aniCon = 0;
	}
}

bool SkeletalMesh::Load(const char* fileName)
{
	Hierarchy* tempMH = new Hierarchy;

	HRESULT hr = D3DXLoadMeshHierarchyFromXA(fileName, D3DXMESH_MANAGED, m_D3DDevice, tempMH, NULL, &m_rootFrame, &m_aniCon);

	if (hr != S_OK)
	{
		ErrorMessage("Unable to load mesh hierarchy");
		return false;
	}

	delete tempMH;
	tempMH = 0;

	if (m_aniCon)
	{
		m_numberOfAnims = m_aniCon->GetMaxNumAnimationSets();
	}

	if (m_rootFrame)
	{
		SetupBone((Frame*)m_rootFrame, NULL);
		m_boneMatrices = new Matrix[m_maxBones];
		ZeroMemory(m_boneMatrices, sizeof(Matrix)*m_maxBones);
	}
	return true;
}

void SkeletalMesh::Render()
{
	Matrix ident;
	MatrixIdentity(&ident);

	if (m_rootFrame)
	{
		DrawFrame(m_rootFrame);
	}
}

void SkeletalMesh::RenderSkeleton()
{
	if (m_rootFrame)
	{
		DrawSkeleton(m_rootFrame, NULL);
	}
}

void SkeletalMesh::Update(float elapsedTime)
{
	if (m_aniCon)
	{
		m_aniCon->AdvanceTime(elapsedTime, NULL);
	}


	Matrix ident;
	MatrixIdentity(&ident);
	UpdateFrame(m_rootFrame, &ident);
}

void SkeletalMesh::UpdateFrame(const D3DXFRAME* base, const Matrix* parentMatrix)
{
	Frame* currentFrame = (Frame*)base;

	if (parentMatrix != NULL)
	{
		MatrixMultiply(&currentFrame->combinedTransform, &currentFrame->TransformationMatrix, parentMatrix);
	}
	else
	{
		currentFrame->combinedTransform = currentFrame->TransformationMatrix;
	}

	if (currentFrame->pFrameSibling != NULL)
	{
		UpdateFrame(currentFrame->pFrameSibling, parentMatrix);
	}

	if (currentFrame->pFrameFirstChild != NULL)
	{
		UpdateFrame(currentFrame->pFrameFirstChild, &currentFrame->combinedTransform);
	}
}

void SkeletalMesh::SetupBone(Frame* frame, const Matrix* parentTransform)
{
	MeshContainer* tempMesh = (MeshContainer*)frame->pMeshContainer;

	if (tempMesh)
	{
		if (tempMesh->pSkinInfo && tempMesh->m_mesh)
		{
			m_maxBones = max(m_maxBones, (int)tempMesh->pSkinInfo->GetNumBones());
			for (unsigned int i = 0; i < tempMesh->pSkinInfo->GetNumBones(); i++)
			{
				Frame* tempFrame = (Frame*)D3DXFrameFind(m_rootFrame, tempMesh->pSkinInfo->GetBoneName(i));
				tempMesh->m_frameMatrix[i] = &tempFrame->combinedTransform;
			}
		}
	}
	if (frame->pFrameSibling)
	{
		SetupBone((Frame*)frame->pFrameSibling, parentTransform);
	}
	if (frame->pFrameFirstChild)
	{
		SetupBone((Frame*)frame->pFrameFirstChild, &frame->combinedTransform);
	}
}

/*This is where the frames are iterated through it is similar to
DrawSkeleten except we are calling DrawContainer to actually skin the bones
*/
void SkeletalMesh::DrawFrame(LPD3DXFRAME frame)
{
	LPD3DXMESHCONTAINER tempContainer = frame->pMeshContainer;
	while (tempContainer != NULL)
	{

		DrawContainer(tempContainer, frame);
		tempContainer = tempContainer->pNextMeshContainer;
	}
	if (frame->pFrameSibling != NULL)
	{
		DrawFrame(frame->pFrameSibling);
	}

	if (frame->pFrameFirstChild != NULL)
	{
		DrawFrame(frame->pFrameFirstChild);
	}
}

void SkeletalMesh::DrawSkeleton(LPD3DXFRAME frame, const Matrix* position)
{
	Frame* tempframe = (Frame*)frame;
	const Matrix& frameMat = tempframe->combinedTransform;
	if (frame != m_rootFrame)
	{

		if (position)
		{
			Vector3D posB = Vector3D(frameMat._41, frameMat._42, frameMat._43);
			Vector3D posA = Vector3D(position->_41, position->_42, position->_43);

			Line boneLine = Line(posA, posB, 0xffff00ff, 0xffff00ff);
			boneLine.Render();
		}
	}

	if (frame->pFrameSibling != NULL)
	{
		DrawSkeleton(frame->pFrameSibling, position);
	}

	if (frame->pFrameFirstChild != NULL)
	{
		DrawSkeleton(frame->pFrameFirstChild, &frameMat);
	}
}

/* This is where the skinning actually happens
This demo uses software skinning so the skin vertices are transformed by the
bone matrices and copied into a secondary mesh which is then rendered
*/

void SkeletalMesh::DrawContainer(LPD3DXMESHCONTAINER container, LPD3DXFRAME frame)
{
	Frame* tempframe = (Frame*)frame;

	MeshContainer* tempContainer = (MeshContainer*)container;

	DWORD       cBones = tempContainer->pSkinInfo->GetNumBones();
	Matrix  identity;
	PBYTE       pbVerticesSrc;
	PBYTE       pbVerticesDest;

	if (tempContainer->pSkinInfo != NULL)
	{
		////// set up bone transforms
		for (DWORD iBone = 0; iBone < cBones; ++iBone)
		{
			MatrixMultiply(&m_boneMatrices[iBone], &tempContainer->m_boneOffset[iBone], tempContainer->m_frameMatrix[iBone]);
		}

		/*
		tempContainer->m_mesh contains the default skin whilst tempContainer->MeshData.pMesh
		contains a tempory skin we copy the vertices into the tempory mesh and then transform it
		using the bone matrices
		*/

		tempContainer->m_mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc);

		tempContainer->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pbVerticesDest);

		//// generate skinned mesh
		tempContainer->pSkinInfo->UpdateSkinnedMesh(m_boneMatrices, NULL, pbVerticesSrc, pbVerticesDest);

		tempContainer->MeshData.pMesh->UnlockVertexBuffer();

		tempContainer->m_mesh->UnlockVertexBuffer();

		/*
		Draw the mesh
		*/
		for (unsigned int i = 0; i < tempContainer->NumMaterials; i++)
		{
			m_D3DDevice->SetMaterial(&tempContainer->m_material[i]);
			m_D3DDevice->SetTexture(0, tempContainer->m_textures[i]);
			tempContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
	else
	{
		m_D3DDevice->SetTransform(D3DTS_WORLD, &tempframe->combinedTransform);

		for (unsigned int i = 0; i < tempContainer->NumMaterials; i++)
		{
			m_D3DDevice->SetMaterial(&tempContainer->m_material[i]);
			m_D3DDevice->SetTexture(0, tempContainer->m_textures[i]);
			tempContainer->MeshData.pMesh->DrawSubset(0);
		}
	}
}
