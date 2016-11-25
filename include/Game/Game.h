#pragma once

#include "Window.h"
#include "Managers\TextureManager.h"
#include "Managers\StateManager.h"
#include "SharedContext.h"
#include "Managers\EntityManager.h"
#include "Camera.h"
#include "Managers\AudioManager.h"
#include "Managers\GUIManager.h"
#include "Managers\FontManager.h"

class Game
{
public:
	Game();
	~Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	bool isRunning() const { return m_window.isRunning(); }

	void update();
	void draw();
	void lateUpdate();

private:
	//Remember that these classes destructors are called from bottom to top
	Window m_window;
	FontManager m_fontManager;
	GUIManager m_guiManager;
	AudioManager m_audioManager;
	TextureManager m_textureManager;
	SharedContext m_sharedContext;
	StateManager m_stateManager;
	EntityManager m_entityManager;
	
	sf::Time m_timeElasped;
	sf::Clock m_clock;
};

