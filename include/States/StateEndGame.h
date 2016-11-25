#pragma once
#include "StateBase.h"
class StateEndGame :
	public StateBase
{
public:
	StateEndGame(StateManager& stateManager, const StateType stateType);
	~StateEndGame();

	void draw(sf::RenderWindow& window) override {}
};

