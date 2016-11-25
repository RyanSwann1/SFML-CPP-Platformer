#include "States\StateEndGame.h"
#include "SharedContext.h"
#include "Managers\StateManager.h"
#include "Window.h"

StateEndGame::StateEndGame(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	Window& window = *m_stateManager->getSharedContext().m_window;
	window.close();
	window.stopRunning();
}

StateEndGame::~StateEndGame()
{
}
