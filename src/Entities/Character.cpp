#include "Entities\Character.h"
#include "SharedContext.h"
#include "Game\Map.h"
#include "Managers\EntityManager.h"
#include "Animation\AnimationName.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

Character::Character(const SharedContext& sharedContext, const std::string& name)
	: Entity(sharedContext, name),
	m_jumpReady(true),
	m_hurt(false),
	m_attackManager(sharedContext),
	m_lives(0),
	m_damage(0),
	m_hurtTime(0),
	m_jumpTime(0),
	m_jumpVelocity(0),
	m_maxLives(0)
{
	m_attackAABB.height = Sheet::TILE_SIZE;
	m_attackAABB.width = Sheet::TILE_SIZE;
}

Character::~Character()
{}

void Character::load(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Unable to open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string type;
		
		keyStream >> type;
		if (type == "SpriteSheetDetails")
		{
			std::string spriteSheetName;
			keyStream >> spriteSheetName;
			getSpriteSheet().loadIn(spriteSheetName);
			getSpriteSheet().loadIn(spriteSheetName);
		}
		else if (type == "MaxLives")
		{
			keyStream >> m_maxLives;
			m_lives = m_maxLives;
		}
		else if (type == "Damage")
		{
			keyStream >> m_damage;
		}
		else if (type == "Speed")
		{
			sf::Vector2f speed;
			keyStream >> speed.x >> speed.y;
			Entity::setSpeed(speed);
		}
		else if (type == "JumpVelocity")
		{
			keyStream >> m_jumpVelocity;
		}
		else if (type == "MaxVelocity")
		{
			sf::Vector2f maxVel;
			keyStream >> maxVel.x >> maxVel.y;
			Entity::setMaxVelocity(maxVel);
		}
		else if (type == "Position")
		{
			sf::Vector2f pos;
			keyStream >> pos.x >> pos.y;
			Entity::setPosition(pos);
		}
		else if (type == "JumpTime")
		{
			keyStream >> m_jumpTime;
			m_jumpingTimer.setExpirationTime(m_jumpTime);
		}
		else if (type == "HurtTime")
		{
			keyStream >> m_hurtTime;
			m_hurtTimer.setExpirationTime(m_hurtTime);
		}
	}
}

void Character::update(const float deltaTime)
{
	Entity::update(deltaTime);

	if (m_currentState != EntityState::Attacking &&
		m_currentState != EntityState::Hurt &&
		m_currentState != EntityState::Dead)
	{
		if (std::abs(Entity::getVelocity().x) >= 0.5f)
		{
			setState(EntityState::Walking);
		}
		else 
		{
			setState(EntityState::Idle);
		}
	}

	handleTimers(deltaTime);
	getSpriteSheet().update(*this, deltaTime);
}

void Character::moveInDirection(const Direction dir)
{
	if (m_currentState == EntityState::Hurt ||
		m_currentState == EntityState::Dead ||
		m_currentState == EntityState::Attacking)
	{
		return;
	}
	Entity::moveInDirection(dir);
}

void Character::reduceLife(const int damage)
{
	//Only allow character to take damage periodic intervals
	if (m_currentState == EntityState::Dead || m_hurtTimer.isActive()) {
		return;
	}

	if ((m_lives - damage) <= 0)
	{
		killCharacter();
		m_lives = 0;
		//m_velocity.x = 0;
		//setState(EntityState::Dead);
		//m_spriteSheet.setAnimationType("Dead", m_currentDirection);	
	}
	else 
	{
		getSpriteSheet().setAnimationType(AnimationName::Hurt, getDirection());
		getAudioPlayer().play("Hurt", false);
		setState(EntityState::Hurt);
		m_lives -= damage;
		m_hurtTimer.activate(); 
		Entity::setVelocity(0, Entity::getVelocity().y);
	}
}

void Character::jump()
{
	if (m_jumpReady && m_currentState != EntityState::Dead)
	{
		getAudioPlayer().play("Jump", false);
		m_jumpReady = false;
		m_jumpingTimer.activate();
	}
}

void Character::killCharacter()
{
	if (m_currentState != EntityState::Dead)
	{
		getAudioPlayer().play("Hurt", false);
		setState(EntityState::Dead);
		getSpriteSheet().setAnimationType(AnimationName::Dead, getDirection());
		Entity::setVelocity(0, 0);
	}
}

void Character::handleTimers(const float deltaTime)
{
	m_jumpingTimer.update(deltaTime);
	if (m_jumpingTimer.isActive())
	{
		addVelocity(0, -m_jumpVelocity);
	}
	m_hurtTimer.update(deltaTime);
}

void Character::setState(const EntityState newEntityState)
{
	if (m_currentState != EntityState::Dead)
	{
		m_currentState = newEntityState;
	}
}

void Character::resolveCollisions(std::vector<CollisionElement*>& collisions)
{
	Entity::resolveCollisions(collisions);
	if (isCollidingOnY())
	{
		m_jumpReady = true;
	}
}

void Character::attack()
{
	//Disallow attacking within these states
	if (m_currentState == EntityState::Attacking ||
		m_currentState == EntityState::Dead ||
		m_currentState == EntityState::Hurt)
	{
		return;
	}

	setState(EntityState::Attacking);
	getSpriteSheet().setAnimationType(AnimationName::Attack, getDirection());
	getAudioPlayer().play("Attack", false);
	stop();
	m_attackManager.startAttack(*this);
}
