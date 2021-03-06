#pragma once
#include "States\StateBase.h"

struct EventDetails;
class StateGameOver :
	public StateBase
{
public:
	StateGameOver(StateManager& stateManager, const StateType stateType);
	~StateGameOver();

	void onEnter() override;
	void onExit() override;

	void draw(sf::RenderWindow& window) override;

private:
	void reactToInput(const EventDetails& eventDetails);
	void onMouseClick();
};

