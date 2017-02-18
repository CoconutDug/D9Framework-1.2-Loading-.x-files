#include "GameState.h"

GameState::GameState(std::wstring stateName)
	:
	m_stateName(stateName)
{
}

std::wstring GameState::GetStateName() const
{
	return m_stateName;
}