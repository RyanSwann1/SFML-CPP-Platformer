#pragma once

#include "States\StateType.h"
#include <unordered_map>
#include <vector>
#include <list>
#include <functional>
#include <SFML\Graphics.hpp>

//enum class StateType;
class StateBase;
struct SharedContext;
class StateManager
{
public:
	explicit StateManager(SharedContext& sharedContext);
	~StateManager();

	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;

	SharedContext& getSharedContext() const { return *m_sharedContext; }
	const StateBase& getCurrentState() const { return *m_currentState; }

	void switchTo(const StateType stateType);
	void removeState(const StateType stateType);
	void update(const float deltaTime);
	void draw(sf::RenderWindow& window);
	void createState(const StateType stateType);

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
};