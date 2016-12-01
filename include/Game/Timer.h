#pragma once

#include <iostream>

class Timer
{
public:
	Timer();
	~Timer();

	inline void setExpirationTime(const float expirationTime) { m_expirationTime = expirationTime; }
	inline void activate() { m_isActive = true; }
	
	inline void resetTime() { m_timeElasped = 0; }
	void update(const float elaspedTime);
	void deactivate();

	inline bool isExpired() const { return m_timeElasped >= m_expirationTime; }
	inline bool isActive() const { return m_isActive; }

private:
	float m_timeElasped;
	float m_expirationTime;
	bool m_isActive;
};

