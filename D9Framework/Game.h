#pragma once
#include "GameState.h"
#include "Model.h"
#include "Scene.h"
#include "Cube.h"
#include <memory>

class GameState;
class Stats;
class Text;
class Scene;

class Game : public GameState
{
public:
	Game();
	virtual	~Game();
	virtual void OnEntry();
	virtual void OnExit();
	virtual void Update(float elapsedTime);
	virtual void Render();
public:
	//Stats* m_stats;
	std::unique_ptr<Scene> m_scene;
	Matrix m_world;
	std::unique_ptr<Model> car;
	//Matrix m_view;
	//Matrix m_proj;
	//Text* m_Text;
	std::unique_ptr<Cube> cube;
};


