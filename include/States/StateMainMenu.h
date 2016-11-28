#pragma once
#include "States\StateBase.h"

class StateManager;
struct EventDetails;
enum class StateType;
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