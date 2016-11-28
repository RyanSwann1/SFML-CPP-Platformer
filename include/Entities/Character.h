#pragma once

#include "Entities\Entity.h"
#include "Managers\AttackManager.h"
#include "Game\Timer.h"
#include "EntityState.h"
#include <iostream>
#include <SFML\Graphics.hpp>

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
	
	void reduceLife(const int damage);

protected:
	void jump();
	void move(const Direction dir);
	void load(const std::string& fileName);
	inline void changeDirection()
	{
		(Entity::getDirection() == Direction::Left ? Entity::setDirection(Direction::Right) : Entity::setDirection(Direction::Left));
	}
	void killCharacter(); //Set to destroy this character
	void resolveCollisions(std::vector<CollisionElement*>& collisions) override;
	void attack();
	inline void setDirection(const Direction newDir)
	{
		if (getDirection() != newDir)
		{
			Entity::setDirection(newDir);
			stop();
		}
	}
	void update(const float deltaTime) override;

private:
	Timer m_jumpingTimer;
	Timer m_hurtTimer;
	EntityState m_currentState;
	AttackManager m_attackManager;
	sf::FloatRect m_attackAABB;
	
	int m_maxLives;
	int m_lives;
	int m_damage;
	float m_hurtTime; 
	float m_jumpTime;
	float m_jumpVelocity;
	bool m_jumpReady;
	bool m_hurt;

	void setState(const EntityState newEntityState);
	void handleTimers(const float deltaTime);
};

