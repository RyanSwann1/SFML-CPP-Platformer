#include "States\StateGameOver.h"
#include "Managers\StateManager.h"
#include "Managers\GUIManager.h"
#include "SharedContext.h"
#include "Managers\EventManager.h"
#include "Window.h"

StateGameOver::StateGameOver(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	
	FontManager& fontManager = *m_stateManager->getSharedContext().m_fontManager;
	GUIInterface* const gameOverInterface = new GUIInterface(StateType::GameOver);
	gameOverInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Game Over!", sf::Vector2f(400, 200));
	gameOverInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Retry", sf::Vector2f(400, 500));
	gameOverInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Main Menu", sf::Vector2f(400, 650));

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.addInterface(StateType::GameOver, *gameOverInterface);
}

StateGameOver::~StateGameOver()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.removeInterface(StateType::GameOver);
}


void StateGameOver::onEnter()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StateGameOver>(KeyBinding::Left_Click, StateType::GameOver, &StateGameOver::reactToInput, this);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility(StateType::GameOver, true);
	guiManager.setCurrentInterface(StateType::GameOver);
}

void StateGameOver::onExit()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.removeCallback(KeyBinding::Left_Click);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility(StateType::GameOver, false);
}

void StateGameOver::draw(sf::RenderWindow & window)
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.draw(window);
}

void StateGameOver::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_keyBinding == KeyBinding::Left_Click)
	{
		onMouseClick();
	}
}

void StateGameOver::onMouseClick()
{
	//const GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	//sf::RenderWindow& window = m_stateManager->getSharedContext().m_window->getWindow();
	//const sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	//if (guiManager.interactingWithGuiEntry(mousePos, "Retry"))
	//{
	//	m_stateManager->switchTo(StateType::Game);
	//	m_stateManager->removeState(StateType::GameOver);
	//}
	//else if (guiManager.interactingWithGuiEntry(mousePos, "Main Menu"))
	//{
	//	m_stateManager->switchTo(StateType::MainMenu);
	//	m_stateManager->removeState(StateType::GameOver);
	//}
}