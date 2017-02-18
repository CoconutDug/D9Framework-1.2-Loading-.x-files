#pragma once
#include "Model.h"
#include "Quad.h"
#include "Sphere.h"
#include "SkeletalMesh.h"

class Scene
{
public:
	Scene();
	~Scene();
	void CreateScene();
	void Update(float elapsedTime);
	void Render();
private:
	enum Target
	{
		NONE = 0,
		CAR1,
		CAR2,
		SPHERE
	}
	m_viewTarget;

	std::unique_ptr<Quad> m_floor;
	std::unique_ptr<Model> m_car1;
	std::unique_ptr<SkeletalMesh> m_mesh;
	std::unique_ptr<Sphere> m_sphere;
	float m_timer;
};