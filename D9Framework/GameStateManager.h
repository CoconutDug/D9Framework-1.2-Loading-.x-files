#pragma once
#include "Types.h"
#include "GameState.h"
#include <memory>
#include <vector>

class GameStateManager : public Singleton<GameStateManager>
{
public:
	GameStateManager();
	~GameStateManager();
	void AddState(std::shared_ptr<GameState> state);
	void SwitchState(std::wstring stateName);
	std::wstring	GetCurrentStateName() const;
	void Update(float elapsedTime);
	void Render();
	std::shared_ptr<GameState> GetCurrentState() const;
private:
	static const int c_MaxNumStates = 10;
	std::shared_ptr<GameState> m_currentState;
	std::vector<std::shared_ptr<GameState>> m_stateVector;
	int m_numStates;
	bool m_updating;
	bool m_cachedSwitch;
	std::wstring m_cachedSwitchState;
};

