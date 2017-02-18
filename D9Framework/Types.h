#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "Singleton.h"

#define RED 0x00ff0000
#define BLUE 0x000000ff
#define GREEN 0x0000ff00

#define _CRTDBG_MAP_ALLOC

#define Vector2DLength D3DXVec2Length

#define Vector3DNormalize D3DXVec3Normalize
#define Vector3DCrossProduct D3DXVec3Cross
#define Vector3DDotProduct D3DXVec3Dot
#define Vector3DTransformCoord D3DXVec3TransformCoord
#define Vector3DLength D3DXVec3Length
#define Vector3DLengthSq D3DXVec3LengthSq

#define MatrixIdentity D3DXMatrixIdentity
#define MatrixTranslation D3DXMatrixTranslation
#define MatrixLookAtLH D3DXMatrixLookAtLH
#define MatrixPerspectiveFovLH D3DXMatrixPerspectiveFovLH
#define MatrixRotationAxis D3DXMatrixRotationAxis
#define MatrixRotationX D3DXMatrixRotationX
#define MatrixRotationY D3DXMatrixRotationY
#define MatrixRotationZ D3DXMatrixRotationZ
#define MatrixInverse D3DXMatrixInverse
#define MatrixMultiply D3DXMatrixMultiply
#define MatrixRotationYawPitchRoll D3DXMatrixRotationYawPitchRoll
#define MatrixScaling D3DXMatrixScaling

#define ASSETMODELPATH "\\Assets\\Models\\"
#define ASSETBASETEXTUREPATH "\\Assets\\Textures\\BaseMaps\\"
#define ASSETNORMALMAPTEXTUREPATH "\\Assets\\Textures\\NormalMaps\\"
#define ASSETEFFECTPATH "\\Assets\\Effects\\"
#define ASSETPARTICALSTEXTUREPATH "\\Assets\\Textures\\Particals\\"

typedef D3DXVECTOR2 Vector2D;
typedef D3DXVECTOR3 Vector3D;
typedef D3DXVECTOR4 Vector4D;

typedef D3DXMATRIX Matrix;
typedef D3DXQUATERNION Quaternion;

typedef ID3DXMesh Mesh;
typedef D3DXCOLOR Colour;
typedef IDirect3DTexture9 Texture;

extern LPDIRECT3DDEVICE9 m_D3DDevice;

static int GetRandomInt(int max)
{
	return rand() % max;
}

static float GetRandomFloat(float a, float b)
{
	if (a >= b)
	{
		float t = a;
		a = b;
		b = t;
	}

	// Get random float in [0, 1] interval.
	float f = (float)((double)rand() / (double)RAND_MAX);

	return (f*(b - a)) + a;
}

static float lerp(float a, float b, float f)
{
	return a + (b - a) * f;
}

static float clamp(float a, float min, float max)
{
	return a < min ? min : (a > max ? max : a);
}
