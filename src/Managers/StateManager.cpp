#include "Managers\StateManager.h"
#include "Window.h"
#include "States\StateGame.h"
#include "States\StateMainMenu.h"
#include "States\StateEndGame.h"
#include "States\StateGameOver.h"
#include "States\StatePause.h"
#include "States\StateType.h"
#include <algorithm>

StateManager::StateManager(SharedContext& sharedContext)
	: m_sharedContext(&sharedContext),
	m_currentState(nullptr)
{
	registerState<StateGame>(StateType::Game);
	registerState<StateMainMenu>(StateType::MainMenu);
	registerState<StateEndGame>(StateType::EndGame);
	registerState<StateGameOver>(StateType::GameOver);
	registerState<StatePause>(StateType::Paused);
}

StateManager::~StateManager()
{
	purgeStates();	
}

void StateManager::switchTo(const StateType stateType)
{
	//Exit the current state
	if (m_currentState)
	{
		m_currentState->onExit();
	}
	//Iterate through existing states to see if state already exists
	auto iter = std::find_if(m_states.begin(), m_states.end(), [stateType](const std::pair<StateType, StateBase*>& state ) {return state.second->getType() == stateType; });
	if (iter != m_states.end())
	{
		//Change current state and enter new
		m_currentState = iter->second;
		m_currentState->onEnter();
		return;
	}

	//for (auto iter = m_states.begin(); iter != m_states.end(); ++iter)
	//{
	//	if (stateType == iter->first)
	//	{
	//		m_currentState->onExit();
	//		const StateType tempType = iter->first;
	//		StateBase* const tempState = iter->second;
	//		m_states.emplace_back(std::make_pair(tempType, tempState));
	//		m_currentState = m_states.back().second;
	//		m_currentState->onEnter();
	//		return;
	//	}
	//}

	createState(stateType);
	m_currentState = m_states.back().second;
	m_currentState->onEnter();
	m_sharedContext->m_window->getWindow().setView(m_view);
}

void StateManager::removeState(const StateType stateType)
{
	if (std::find(m_removals.cbegin(), m_removals.cend(), stateType) == m_removals.cend())
	{
		m_removals.push_back(stateType);
	}
}

void StateManager::update(const float deltaTime)
{
	if (m_currentState)
	{
		m_currentState->update(deltaTime);
	}
	
	processRemovals();
}

void StateManager::draw(sf::RenderWindow & window)
{
	if (m_currentState)
	{
		m_currentState->draw(window);
	}
	
}
void StateManager::createState(const StateType stateType)
{
	//Create new state
	auto iter = m_stateFactory.find(stateType);
	if (iter != m_stateFactory.cend())
	{
		m_states.emplace_back(std::make_pair(iter->first, iter->second()));
	}
}

void StateManager::processRemovals()
{
	for (auto iter = m_removals.begin(); iter != m_removals.end();)
	{
		if (removeActiveState(*iter))
		{
			iter = m_removals.erase(iter);
		}
		else 
		{
			++iter;
		}
	}
}

void StateManager::purgeStates()
{
	for (auto &i : m_states)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_states.clear();
}

bool StateManager::removeActiveState(const StateType stateType)
{
	auto iter = std::find_if(m_states.begin(), m_states.end(), [stateType](const std::pair<StateType, StateBase*>& state) {return state.second->getType() == stateType; });
	if (iter != m_states.end())
	{
		delete iter->second;
		iter->second = nullptr;
		m_states.erase(iter);
		return true;
	}

	return false;
}