#pragma once

#include "Direction.h"
#include "AnimationName.h"
#include <SFML\Graphics.hpp>
#include <array>
#include <string>
#include <unordered_map>

enum class EntityState;
struct SharedContext;
class Animation
{
public:
	friend class SpriteSheet;
	explicit Animation(const int name, const int startFrame, const int endFrame, const int row, const float frameTime, const int direction, const int repeatable);

	void update(const float deltaTime);
	sf::Vector2i getDrawTargetPos() const { return sf::Vector2i(m_currentFrame, m_row); }
	const AnimationName getName() const { return m_name; }
	const Direction getDirection() const { return m_direction; }

	inline void play() { m_isPlaying = true; }
	inline void stop()
	{
		m_isPlaying = false;
		m_currentFrame = 0;
		m_timeElasped = 0;
	}
	inline void pause()
	{
		m_isPlaying = false;
		m_pausedTime = m_timeElasped;
	}
	inline void unPause()
	{
		m_isPlaying = true;
		m_pausedTime = 0;
	}
	const inline bool isFinished() const { return m_currentFrame == m_endFrame; }
	const inline bool repeatable() const { return m_repeatable; }

private:
	const AnimationName m_name;
	const Direction m_direction;
	const int m_startFrame;
	const int m_endFrame;
	const int m_row;
	float m_timeElasped;
	float m_pausedTime;
	float m_frameTime;
	int m_currentFrame;
	bool m_repeatable;
	bool m_isPlaying;
};
