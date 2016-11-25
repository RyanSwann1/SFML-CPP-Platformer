#include "States\StatePause.h"
#include "Managers\GUIManager.h"
#include "SharedContext.h"
#include "Managers\StateManager.h"
#include "Managers\FontManager.h"
#include "Managers\EventManager.h"
#include "Window.h"

StatePause::StatePause(StateManager& stateManager, const StateType stateType)
	: StateBase(stateManager, stateType)
{
	FontManager& fontManager = *m_stateManager->getSharedContext().m_fontManager;
	GUIInterface* const pausedInterface = new GUIInterface(StateType::Paused);
	pausedInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Paused", sf::Vector2f(400, 200));
	pausedInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Resume", sf::Vector2f(400, 550));
	pausedInterface->constructNewGuiElement(fontManager, GuiStyle(sf::Color::White, 18, sf::Vector2f(150, 50), "Magnificent"), "Quit", sf::Vector2f(400, 650));
	
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.addInterface(StateType::Paused, *pausedInterface);
}

StatePause::~StatePause()
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.removeInterface(StateType::Paused);
}

void StatePause::onEnter()
{
	EventManager& eventManager = *m_stateManager->getSharedContext().m_eventManager;
	eventManager.addCallback<StatePause>(KeyBinding::Left_Click, StateType::Paused, &StatePause::reactToInput, this);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setCurrentInterface(StateType::Paused);
	guiManager.setInterfaceVisibility(StateType::Paused, true);
}

void StatePause::onExit()
{
	EventManager& eventManger = *m_stateManager->getSharedContext().m_eventManager;
	eventManger.removeCallback(KeyBinding::Left_Click);

	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.setInterfaceVisibility(StateType::Paused, false);
}

void StatePause::draw(sf::RenderWindow & window)
{
	GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;
	guiManager.draw(window);
}

void StatePause::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_keyBinding == KeyBinding::Left_Click)
	{
		onMouseClick();
	}
}

void StatePause::onMouseClick()
{
	const GUIManager& guiManager = *m_stateManager->getSharedContext().m_guiManager;

	sf::RenderWindow& window = *&m_stateManager->getSharedContext().m_window->getWindow();
	const sf::Vector2f mousePos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	if (guiManager.interactingWithGuiEntry(mousePos, "Resume"))
	{
		m_stateManager->switchTo(StateType::Game);
	}
	else if (guiManager.interactingWithGuiEntry(mousePos, "Quit"))
	{
		m_stateManager->switchTo(StateType::MainMenu);
		m_stateManager->removeState(StateType::Paused);
		m_stateManager->removeState(StateType::Game);
	}
}