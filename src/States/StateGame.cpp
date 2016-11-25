#include "States\StateGame.h"
#include "SharedContext.h"
#include "Managers\StateManager.h"
#include "Managers\EntityManager.h"
#include "Window.h"
#include "Entities\Character.h"
#include "Managers\EventManager.h"

StateGame::StateGame(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType),
	m_camera(stateManager.getSharedContext()),
	m_map(stateManager.getSharedContext()),
	m_audioPlayer(stateManager.getSharedContext()),
	m_paused(false)
{
	m_map.loadNextLevel();
	m_audioPlayer.registerAudioClip("Game", "BackgroundMusic");
	m_audioPlayer.registerOwner("Game");

	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StateGame>(KeyBinding::Pause, StateType::Game, &StateGame::reactToInput, this);
}

StateGame::~StateGame()
{
	//SharedContext& sharedContext = m_stateManager->getSharedContext();
	m_stateManager->getSharedContext().m_map = nullptr;
	m_stateManager->removeState(StateType::Paused);
	m_stateManager->removeState(StateType::Game);
	m_map.purgeTileMap();
	m_map.purgeTileSet();
}

void StateGame::onEnter()
{
	//m_audioPlayer.play("BackgroundMusic", false);
	m_paused = false;
}

void StateGame::onExit()
{
	m_camera.reset();
	m_audioPlayer.stop("BackgroundMusic");
}

void StateGame::update(const float deltaTime)
{
	//Do not update game if paused
	if (m_paused) {
		return;
	}
	m_map.update();

	const SharedContext& sharedContext = m_stateManager->getSharedContext();
	//Set position for the state's view
	const Character* const player = static_cast<Character*>(sharedContext.m_entityManager->findEntity("Player"));
	if (player)
	{
		m_camera.update(*player);
	}
	//If player does not exist, spawn new player
	else
	{	
		m_camera.reset();
		if (!m_map.loadingNextMap())
		{
			m_stateManager->switchTo(StateType::GameOver);
			m_stateManager->removeState(StateType::Game);
		}	
	}
	
	sharedContext.m_entityManager->update(deltaTime);
}

void StateGame::draw(sf::RenderWindow & window)
{
	const SharedContext& sharedContext = m_stateManager->getSharedContext();
	m_map.draw(window);
	
	sharedContext.m_entityManager->draw(window);
}

void StateGame::reactToInput(const EventDetails& eventDetails)
{
	if(eventDetails.m_keyBinding == KeyBinding::Pause)
	{ 
		m_paused = true;
		//m_stateManager->switchTo(StateType::Paused);
		m_stateManager->switchTo(StateType::MainMenu);
		m_stateManager->removeState(StateType::Game);
	}
}