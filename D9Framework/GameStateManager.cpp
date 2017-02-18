#include "GameStateManager.h"
#include "Vertex.h"
#include "Program.h"
#include "Error.h"

DefineSingleton(GameStateManager)

GameStateManager::GameStateManager() 
	:
	m_numStates(0),
	m_currentState(0),
	m_updating(false),
	m_cachedSwitch(false)
{
	InitAllVertexDeclarations();
}

GameStateManager::~GameStateManager()
{
	DestroyAllVertexDeclarations();

	if (m_currentState)
	{
		m_currentState->OnExit();
		m_currentState = 0;
	}

	m_stateVector.clear();
}

void GameStateManager::AddState(std::shared_ptr<GameState>state)
{
	for (auto i = m_stateVector.begin(), end = m_stateVector.end(); i != end; i++)
	{
		if ((*i)->GetStateName().compare(state->GetStateName()) == 0)
		{
			ErrorMessage("This State Already Exists");
			//state already exists
		}
	}

	assert(m_numStates < c_MaxNumStates);
	m_stateVector.push_back(state);
	m_numStates++;

	if (m_currentState == 0)
	{
		m_currentState = m_stateVector.back();
		m_currentState->OnEntry();
	}
}
void GameStateManager::SwitchState(std::wstring stateName)
{
	if (m_updating)
	{
		m_cachedSwitch = true;
		m_cachedSwitchState = stateName;
		return;
	}

	if (m_currentState)
	{
		m_currentState->OnExit();
	}

	for (auto i = m_stateVector.begin(), end = m_stateVector.end(); i != end; i++)
	{
		if ((*i)->GetStateName().compare(stateName) == 0)
		{
			m_currentState = (*i);
			m_currentState->OnEntry();
			return;
		}
	}
	assert(false);
}

std::wstring GameStateManager::GetCurrentStateName() const
{
	return m_currentState->GetStateName();
}

void GameStateManager::Update(float elapsedTime)
{
	m_updating = true;
	m_currentState->Update(elapsedTime);
	m_updating = false;

	if (m_cachedSwitch)
	{
		m_cachedSwitch = false;
		SwitchState(m_cachedSwitchState);
	}
}

void GameStateManager::Render()
{
	m_currentState->Render();
}

std::shared_ptr<GameState> GameStateManager::GetCurrentState() const
{
	return m_currentState;
}

