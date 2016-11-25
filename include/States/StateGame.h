#pragma once

#include "StateBase.h"
#include "Game\Camera.h"
#include "Audio\AudioPlayer.h"
#include "Game\Map.h"
#include "EventDetails.h"

class StateGame :
	public StateBase
{
public:
	StateGame(StateManager& stateManager, const StateType stateType);
	~StateGame();

	void onEnter() override;
	void onExit() override;

	void update(const float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

private:
	Map m_map;
	Camera m_camera;
	AudioPlayer m_audioPlayer;
	bool m_paused;

	void reactToInput(const EventDetails& eventDetails); //Attempt to change to reference
};

