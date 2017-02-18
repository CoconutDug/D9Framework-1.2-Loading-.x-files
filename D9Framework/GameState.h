#pragma once
#include "Types.h"
#include <string>

class GameState
{
public:
	GameState(std::wstring stateName);
	virtual	~GameState() {}
	virtual void OnEntry() {}
	virtual void OnExit() {}
	virtual void Update(float elapsedTime) {}
	virtual void Render() {};
	std::wstring GetStateName() const;
private:
	std::wstring m_stateName;
};

