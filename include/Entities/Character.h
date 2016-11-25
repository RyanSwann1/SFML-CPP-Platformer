#pragma once

#include "Direction.h"
#include "Entities\Entity.h"
#include "Animation\AnimationName.h"
#include "Managers\AttackManager.h"
#include "Game\Timer.h"
#include <iostream>

class Character :
	public Entity
{
public:
	friend class AttackManager;
	friend class SpriteSheet;
	Character(const SharedContext& sharedContext, const std::string& name);
	~Character();

	const int getDamageValue() const { return m_damage; }
	const EntityState getState() const { return m_currentState; }
	const int getLives() const { return m_lives; }

	void move(const Direction dir);
	
	inline void setDirection(const Direction newDir)
	{
		if (m_currentDirection != newDir)
		{
			m_currentDirection = newDir;
			Entity::setVelocity(0, Entity::getVelocity().y);
		}
	}

	void update(const float deltaTime) override;
	void reduceLife(const int damage);


protected:
	int m_lives;
	int m_damage;

	EntityState m_currentState;
	AttackManager m_attackManager;
	
	void jump();
	void load(const std::string& fileName);
	//void attackPreperation();
	inline void changeDirection()
	{
		if (m_currentDirection == Direction::Left) {
			m_currentDirection = Direction::Right;
		}
		else if (m_currentDirection == Direction::Right) {
			m_currentDirection = Direction::Left;
		}
	}
	void killCharacter(); //Set to destroy this character
	void resolveCollisions() override;

private:
	Timer m_jumpingTimer;
	Timer m_hurtTimer;
	int m_maxLives;

	float m_hurtTime; 
	float m_jumpTime;
	float m_jumpVelocity;
	bool m_jumpReady;
	bool m_hurt;

	void setState(const EntityState newEntityState);
	//void attack(Entity& entity, const int x, const int y);
	void handleTimers(const float deltaTime);
	void remove();
};

