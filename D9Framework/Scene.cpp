#include "Types.h"
#include "Material.h"
#include "Model.h"
#include "Scene.h"
#include "CameraController.h"
#include "FreeCamera.h"
#include "InputManager.h"
#include "StaticCamera.h"
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"

Scene::Scene() 
	:
	m_timer(0)
{
	CreateScene();
	m_viewTarget = NONE;
	CameraController::GetInstance()->AddCamera(std::make_shared<FreeCamera>());
	CameraController::GetInstance()->GetCurrentCamera()->SetupCamera(Vector3D(0, 0, -5), Vector3D(0, 0, 1), Vector3D(0, 1, 0));
}

Scene::~Scene()
{

}

void Scene::CreateScene()
{
	m_floor = std::make_unique<Quad>(Vector3D(0, 0, 10), Vector3D(0, 0, 0), 10, 10, 128, 0xFF888888);
	m_car1 = std::make_unique<Model>("Car.x", Vector3D(4, 0, 0), Vector3D(0, 0, 0), 0.6f);
	//m_mesh = std::make_unique<SkeletalMesh>("jet.x");
}

void Scene::Update(float elapsedTime)
{
	//ProcessKeyboard();

	m_timer += elapsedTime;
	float x = cosf( m_timer * 2.000f ) * 20.0f;
	float y = sinf( m_timer * 2.000f ) * 10.0f;
	//m_mesh->Update(elapsedTime);
	static Vector3D v = Vector3D(0,2,0);
}

void Scene::Render()
{
	m_D3DDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );
	m_floor->Render();
	m_car1->Render();
	//m_mesh->Render();
}

