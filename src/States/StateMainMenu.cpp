#include "States\StateMainMenu.h"
#include "SharedContext.h"
#include "Managers\StateManager.h"
#include "GUI\GUIInterface.h"
#include "Managers\GUIManager.h"
#include "Window.h"
#include "Managers\EventManager.h"
#include "Managers\FontManager.h"

StateMainMenu::StateMainMenu(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	//Create the main menu interface
	GUIInterface* const mainMenuInterface = new GUIInterface(StateType::MainMenu);
	FontManager& fontManager = *m_stateManager->getSharedContext().m_fontManager;
	mainMenuInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(250, 150), "Magnificent"), "Play", sf::Vector2f(350, 200));
	mainMenuInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(250, 150), "Magnificent"), "Quit", sf::Vector2f(350, 500));

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.addInterface(StateType::MainMenu, *mainMenuInterface);
}

StateMainMenu::~StateMainMenu()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility(StateType::MainMenu, false);
	guiManager.removeInterface(StateType::MainMenu);
}

void StateMainMenu::onEnter()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StateMainMenu>(KeyBinding::Left_Click, StateType::MainMenu, &StateMainMenu::reactToInput, this);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setCurrentInterface(StateType::MainMenu);
	guiManager.setInterfaceVisibility(StateType::MainMenu, true);
}

void StateMainMenu::onExit()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.removeCallback(KeyBinding::Left_Click);
}

void StateMainMenu::draw(sf::RenderWindow & window)
{
	m_stateManager->getSharedContext().m_guiManager->draw(window);
}

void StateMainMenu::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_keyBinding == KeyBinding::Left_Click)
	{
		onMouseClick();
	}
}

void StateMainMenu::onMouseClick()
{
	Window& window = *m_stateManager->getSharedContext().m_window;
	const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window.getWindow()).x, sf::Mouse::getPosition(window.getWindow()).y);
	const GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	
	if (guiManager.interactingWithGuiEntry(mousePos, "Play"))
	{
		m_stateManager->removeState(StateType::MainMenu);
		m_stateManager->switchTo(StateType::Game);
	}
	else if (guiManager.interactingWithGuiEntry(mousePos, "Quit"))
	{
		m_stateManager->removeState(StateType::MainMenu);
		m_stateManager->switchTo(StateType::EndGame);
	}	
}