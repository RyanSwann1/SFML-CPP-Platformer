#include "Managers\EventManager.h"
#include "Window.h"
#include "SharedContext.h"
#include "Managers\StateManager.h"
#include "States\StateBase.h"
#include "States\StateType.h"
#include <fstream>
#include <sstream>
#include <iostream>
	
EventManager::EventManager(const SharedContext& sharedContext)
	: m_sharedContext(&sharedContext)
{
	loadInBindings("Keys.txt");
}

EventManager::~EventManager()
{	
	for (auto &i : m_callBacks)
	{
		i.second.second = nullptr;
	}
	m_callBacks.clear();
}

void EventManager::handleEvent(const sf::Event& polledEvent)
{
	//Search through all bindings to attempt to match the event
	for (auto &bIter : m_bindings)
	{
		//Search through all events for this bind
		for (auto eIter : bIter.m_events)
		{
			const EventType currentEvent(static_cast<EventType>(polledEvent.type));
			//If Event Type and polled event match
			if (eIter.second == currentEvent)
			{
				//If key was polled
				if (polledEvent.type == sf::Event::KeyPressed)
				{
					if (eIter.first.m_keyCode == polledEvent.key.code)
					{
						bIter.m_eventDetails.m_keyCode = eIter.first.m_keyCode;
						bIter.m_eventDetails.m_eventType = EventType::KeyPressed;
						++bIter.m_count;
					}
				}
				//If mouse button was polled
				else if (polledEvent.type == sf::Event::MouseButtonPressed)
				{
					if (eIter.first.m_keyCode == polledEvent.mouseButton.button)
					{
						bIter.m_eventDetails.m_keyCode = polledEvent.mouseButton.button;
						bIter.m_eventDetails.m_eventType = EventType::MouseButtonPressed;
						++bIter.m_count;
					}
				}
				break;
			}
		}
		if (static_cast<EventType>(polledEvent.type) == EventType::KeyReleased)
		{
			bIter.m_eventDetails.m_eventType = EventType::KeyReleased;
		}
		else if (static_cast<EventType>(polledEvent.type) == EventType::MouseButtonReleased)
		{
			bIter.m_eventDetails.m_eventType = EventType::MouseButtonReleased;
		}
	}
}

void EventManager::update()
{
	//Search through bindings
	for (auto &bIter : m_bindings)
	{
		//Check if bindings event has been activated
		if (bIter.m_count)
		{
			//Find callback
			auto iter = m_callBacks.find(bIter.m_keyBinding);
			if (iter != m_callBacks.cend())
			{
				//Make sure to have such data stored in one place. 
				//Do not duplicate incase of forgetting to change external code
				const StateType currentState = m_sharedContext->m_stateManager->getCurrentState().getType();
				//If callback matching the current state, activate
				if (currentState == iter->second.first)
				{
					iter->second.second(bIter.m_eventDetails);
				}
			}

			if (bIter.m_eventDetails.m_eventType == EventType::KeyReleased ||
				bIter.m_eventDetails.m_eventType == EventType::MouseButtonReleased)
			{
				bIter.m_count = 0;
			}	
		}
	}
}

void EventManager::loadInBindings(const std::string & fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "Failed to open: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		int keyBinding = 0;
		int eventType = 0;
		int keyCode = -1;

		keyStream >> keyBinding >> eventType >> keyCode;

		//Will have to change, only supports one added event at each time
		Binding bind(static_cast<KeyBinding>(keyBinding));
		bind.addEvent(EventInfo(keyCode), static_cast<EventType>(eventType));
		m_bindings.push_back(bind);
	}
}

void EventManager::purgeCallbacks()
{
	for (auto &i : m_callBacks)
	{
		i.second.second = nullptr;
	}
	m_callBacks.clear();
}

void EventManager::removeCallback(const KeyBinding keyBinding)
{
	auto iter = m_callBacks.find(keyBinding);
	if (iter != m_callBacks.cend())
	{
		iter->second.second = nullptr;
		m_callBacks.erase(iter);
	}
}