#include "Animation\Animation.h"
#include <iostream>

Animation::Animation(const int name, const int startFrame, const int endFrame, const int row, const float frameTime, const int direction, const int repeatable)
	: m_timeElasped(0),
	m_startFrame(startFrame),
	m_currentFrame(startFrame),
	m_endFrame(endFrame),
	m_row(row),
	m_frameTime(frameTime),
	m_pausedTime(0),
	m_isPlaying(false),
	m_name(static_cast<AnimationName>(name)),
	m_direction(static_cast<Direction>(direction)),
	m_repeatable(repeatable)
{}

void Animation::update(const float deltaTime)
{
	if (m_isPlaying)
	{
		m_timeElasped += deltaTime;

		if (m_timeElasped >= m_frameTime)
		{
			if (m_currentFrame < m_endFrame)
			{
				++m_currentFrame;
			}
			else
			{
				//Only repeat animation if set to do so
				if (m_repeatable)
				{
					m_currentFrame = m_startFrame;
				}
			}
			m_timeElasped = 0;
		}
	}
	//if (m_timeElasped >= m_nextAnimationTime)
	//{
	//	cycleThroughAnimation(m_currentAnimationDirection);
	//	m_timeElasped = 0;
	//}
}