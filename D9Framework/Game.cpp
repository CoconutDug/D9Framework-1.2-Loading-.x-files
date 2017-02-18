#include "Types.h"
#include "Scene.h"
#include "BaseCamera.h"
#include "Game.h"
#include "CameraController.h"
#include "FreeCamera.h"
#include "Sphere.h"
#include "Program.h"
#include "InputManager.h"

Game::Game() 
	:
	GameState(L"Game")
{
	
}

Game::~Game()
{
	CameraController::Destroy();
	//delete m_scene;
}

void Game::OnEntry()
{
	/*m_stats = new Stats();
	m_scene = new Scene();*/

	Program::GetInstance()->SetClearBufferColour(255, 255, 255);
	CameraController::Create();
	m_scene = std::make_unique<Scene>();
	/*std::shared_ptr<BaseCamera> camera = std::make_unique<FreeCamera>();
	CameraController::GetInstance()->AddCamera(camera);

	camera->SetupCamera(Vector3D(0, 0, -15), Vector3D(0, 0, 1), Vector3D(0, 1, 0));
	camera->SetAspectRatio((float)(Program::GetInstance()->GetScreenWidth() / Program::GetInstance()->GetScreenHeight()));
	camera->SetFieldofView(D3DX_PI / 4.f);
	camera->SetNearPlane(0.1f);
	camera->SetFarPlane(5000.0f);*/
	car = std::make_unique<Model>("Car.x", Vector3D(4, 0, 0), Vector3D(0, 0, 0), 0.6f);
}

void Game::OnExit()
{
	CameraController::GetInstance()->RemoveCamera();
	/*delete m_stats;
	delete m_scene;*/

}

void Game::Update(float elapsedTime)
{
	//m_stats->Update(elapsedTime);
	InputManager::GetInstance()->Update();
	CameraController::GetInstance()->Update(elapsedTime);
	m_scene->Update(elapsedTime);
}

void Game::Render()
{
	car->Render();
	CameraController::GetInstance()->ComputeCamera();
	m_scene->Render();
}
