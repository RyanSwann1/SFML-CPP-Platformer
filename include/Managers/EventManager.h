#pragma once

#include "States\StateType.h"
#include "EventDetails.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <SFML\Window.hpp>

struct EventInfo
{
	EventInfo(const int keyCode)
		: m_keyCode(keyCode) {}

	int m_keyCode;
};

struct Binding
{
	Binding(const KeyBinding keyBinding)
		: m_keyBinding(keyBinding),
		m_count(0),
		m_eventDetails(keyBinding)
	{}

	void addEvent(const EventInfo info, const EventType type)
	{
		m_events.emplace_back(info, type);
	}

	std::vector<std::pair<EventInfo, EventType>> m_events;
	const KeyBinding m_keyBinding;
	EventDetails m_eventDetails;
	int m_count;
};

using CallbackContainer = std::pair<StateType, std::function<void(const EventDetails&)>>;
using std::placeholders::_1;
struct SharedContext;
class EventManager
{
public:
	EventManager(const SharedContext& sharedContext);
	~EventManager();

	EventManager(const EventManager&) = delete;
	EventManager& operator=(const EventManager&) = delete;

	//template <class T>
	//void addCallback(const std::string& name, const StateType stateType, void(T::*fp)(const EventDetails*), T* instance)
	//{
	//	if (m_callBacks.find(name) == m_callBacks.cend())
	//	{
	//		auto callBackContainer = std::make_pair(stateType, std::bind(fp, instance, _1));
	//		m_callBacks.emplace(name, callBackContainer);
	//	}
	//}

	template <class T>
	void addCallback(const KeyBinding keyBinding, const StateType stateType, void(T::*fp)(const EventDetails&), T* instance)
	{
		if (m_callBacks.find(keyBinding) == m_callBacks.cend())
		{
			auto callBackContainer = std::make_pair(stateType, std::bind(fp, instance, _1));
			m_callBacks.emplace(std::make_pair(keyBinding, callBackContainer));
		}
	}
	void removeCallback(const KeyBinding keyBinding);

	void handleEvent(const sf::Event& polledEvent);
	void update();

private:
	std::unordered_map<KeyBinding, CallbackContainer> m_callBacks;
	std::vector<Binding> m_bindings;
	const SharedContext* const m_sharedContext;
	void loadInBindings(const std::string& fileName);
	void purgeCallbacks();
};