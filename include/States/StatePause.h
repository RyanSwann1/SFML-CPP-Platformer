#pragma once
#include "States\StateBase.h"

class EventDetails;
class StatePause :
	public StateBase
{
public:
	StatePause(StateManager& stateManager, const StateType stateType);
	~StatePause();

	void onEnter() override;
	void onExit() override;

	void draw(sf::RenderWindow& window);

private:
	void reactToInput(const EventDetails& eventDetails);
	void onMouseClick();
};

