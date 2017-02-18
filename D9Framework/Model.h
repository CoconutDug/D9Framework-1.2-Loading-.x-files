#pragma once
#include "Material.h"
#include <vector>
#include <memory>

class Model
{
public:
	Model(void);
	Model(LPCSTR fileName);
	Model(LPCSTR fileName, Vector3D& position, Vector3D& rotation, float scale);
	~Model(void);
	bool LoadModel(LPCSTR fileName);
	//bool LoadModel(const char* fileName);
	void SetPosition(Vector3D& position);
	void SetPosition(float x, float y, float z);
	Vector3D GetPosition() const;
	void SetRotation(Vector3D rotation);
	void SetRotation(float x, float y, float z);
	Vector3D GetRotation() const;
	void SetScale(float scale);
	float GetScale();
	void Render();
private:
	Vector3D m_position;
	Vector3D m_rotation;
	float m_scale;
	Mesh* m_mesh;
	std::vector<Material> m_materials;
	std::vector<Texture*> m_textures;

};
