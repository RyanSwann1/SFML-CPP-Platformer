#pragma once
#include "States\StateType.h"
#include "Managers\StateManager.h"
#include "EventDetails.h"
class StateMainMenu :
	public StateBase
{
public:
	StateMainMenu(StateManager& stateManager, const StateType stateType);
	~StateMainMenu();

	void onEnter() override;
	void onExit() override;

	void draw(sf::RenderWindow& window) override;

private:
	void reactToInput(const EventDetails& eventDetails);
	void onMouseClick();
};