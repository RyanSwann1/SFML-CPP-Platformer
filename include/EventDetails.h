#pragma once

#include "EventType.h"
#include "KeyBinding.h"

struct EventDetails
{
	EventDetails(const KeyBinding keyBinding)
		: m_keyCode(-1),
		m_keyBinding(keyBinding) {}

	const KeyBinding m_keyBinding;
	EventType m_eventType;
	int m_keyCode;
};