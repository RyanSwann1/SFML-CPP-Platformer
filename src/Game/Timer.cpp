#include "Game\Timer.h"



Timer::Timer()
	: m_timeElasped(0),
	m_expirationTime(0)
{
}

Timer::~Timer()
{
}

void Timer::update(const float deltaTime)
{
	if (m_isActive)
	{
		m_timeElasped += deltaTime;

		if (isExpired())
		{
			deactivate();
		}
	}
}

void Timer::deactivate()
{
	m_isActive = false;
	m_timeElasped = 0;
}