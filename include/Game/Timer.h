#pragma once

#include <iostream>

class Timer
{
public:
	Timer();
	~Timer();

	inline void setExpirationTime(const float expirationTime) { m_expirationTime = expirationTime; }
	inline void activate() { m_isActive = true; }
	inline void deactivate() 
	{
		m_isActive = false; 
		m_timeElasped = 0;
	}
	inline void reset() { m_timeElasped = 0; }
	inline void update(const float elaspedTime) 
	{ 
		if (m_isActive) 
		{
			m_timeElasped += elaspedTime;
		}
	}
	inline bool isExpired() const { return m_timeElasped >= m_expirationTime; }
	inline bool isActive() const { return m_isActive; }

private:
	float m_timeElasped;
	float m_expirationTime;
	bool m_isActive;
};

