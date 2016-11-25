#pragma once

#include "States\StateBase.h"
#include "States\StateType.h"
#include "States\StateMainMenu.h"
#include "SharedContext.h"
#include <unordered_map>
#include <vector>
#include <list>
#include <functional>
#include <SFML\Graphics.hpp>

struct SharedContext;
class StateManager
{
public:
	explicit StateManager(SharedContext& sharedContext);
	~StateManager();

	SharedContext& getSharedContext() const { return *m_sharedContext; }
	const StateBase& getCurrentState() const { return *m_currentState; }
	void switchTo(const StateType stateType);
	void removeState(const StateType stateType);
	void update(const float deltaTime);
	void draw(sf::RenderWindow& window);
	//sf::View& getView() { return m_view; }
	void createState(const StateType stateType);

	////Without templates
	//void registerGameState()
	//{
	//	m_stateFactory.emplace(std::make_pair(StateType::Game, [this]() -> StateBase*
	//	{
	//		return new StateGame(this);
	//	}));
	//}
	//void registerMainMenuState()
	//{
	//	m_stateFactory.emplace(std::make_pair(StateType::MainMenu, [this]() -> StateBase*
	//	{
	//		return new StateMainMenu(this);
	//	}));
	//}

	//With templates
	template <class T>
	void registerState(const StateType stateType)
	{
		if (m_stateFactory.find(stateType) == m_stateFactory.cend())
		{
			m_stateFactory.emplace(std::make_pair(stateType, [this, stateType]() -> StateBase*
			{
				return new T(*this, stateType);
			}));
		}
	}

private:
	std::unordered_map<StateType, std::function<StateBase*()>> m_stateFactory;
	std::list<std::pair<StateType, StateBase*>> m_states;
	std::list<StateType> m_removals;
	StateBase* m_currentState;
	//StateType m_currentStateType;
	SharedContext* m_sharedContext;
	sf::View m_view;
	
	void processRemovals();
	void purgeStates();
	bool removeActiveState(const StateType stateType);
};