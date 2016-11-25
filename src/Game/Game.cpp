#include "Game\Game.h"
#include "Utilities.h"
#include <iostream>


Game::Game()
	: m_stateManager(m_sharedContext),
	m_window(m_sharedContext, sf::Vector2u(1000, 750), "Platformer")
{
	m_sharedContext.m_textureManager = &m_textureManager;
	m_sharedContext.m_stateManager = &m_stateManager;
	m_sharedContext.m_entityManager = &m_entityManager;
	m_sharedContext.m_window = &m_window;
	m_sharedContext.m_eventManager = &m_window.getEventManager();
	m_sharedContext.m_audioManager = &m_audioManager;
	m_sharedContext.m_guiManager = &m_guiManager;
	m_sharedContext.m_fontManager = &m_fontManager;

	m_stateManager.switchTo(StateType::MainMenu);
}

Game::~Game()
{
}

void Game::update()
{
	m_stateManager.update(m_timeElasped.asSeconds());
	m_window.update();
}

void Game::draw()
{
	m_window.beginDraw();
	m_stateManager.draw(m_window.getWindow());
	m_window.endDraw();
}

void Game::lateUpdate()
{
	m_timeElasped = m_clock.restart();
}
