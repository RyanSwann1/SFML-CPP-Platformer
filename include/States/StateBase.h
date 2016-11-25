#pragma once

#include "States\StateType.h"
#include <SFML\Graphics.hpp>

class StateManager;
class StateBase
{
public:
	StateBase(StateManager& stateManager, const StateType stateType)
	: m_stateManager(&stateManager),
	m_type(stateType) {}
	virtual ~StateBase() {}

	StateBase(const StateBase&) = delete;
	StateBase& operator=(const StateBase&) = delete;

	virtual void onEnter() {}
	virtual void onExit() {}

	virtual void update(const float deltaTime) {}
	virtual void draw(sf::RenderWindow& window) = 0;

	sf::View getView() const { return m_view; }
	StateType getType() const { return m_type; }

protected:
	StateManager* m_stateManager;

private:
	sf::View m_view;
	StateType m_type;
};

