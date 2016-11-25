#include "Entities\Rat.h"
#include "SharedContext.h"
#include "Managers\EntityManager.h"
#include "Utilities.h"

Rat::Rat(const SharedContext& sharedContext, const std::string& name)
	: Computer(sharedContext, name),
	m_attackReady(true)
{
	m_type = EntityType::Enemy;
	m_currentDirection = static_cast<Direction>(Utilities::randomNumberGenerator(0, 1));
	m_audioPlayer.registerOwner(name);
	Character::load(m_sharedContext.m_entityManager->getEntityTypeLocation(name));
	m_attackTimer.setExpirationTime(4);
	m_attackTimer.activate();
	Entity::setPosition(sf::Vector2f(400, 400));
}

Rat::~Rat()
{}

void Rat::update(const float deltaTime)
{
	Computer::update(deltaTime);

	handleMovement();
	handleAttack(deltaTime);

	m_attackTimer.update(deltaTime);
	if (m_attackTimer.isExpired())
	{
		m_attackTimer.reset();
		m_attackReady = true;
	}
}

void Rat::onEntityCollision(Entity& entity)
{
	if (m_currentState != EntityState::Dead)
	{
		if (entity.getType() == EntityType::Player)
		{
			const Character& character = static_cast<Character&>(entity);
			if (character.getState() == EntityState::Attacking)
			{
				reduceLife(character.getDamageValue());
			}
		}
	}
}

void Rat::handleAttack(const float deltaTime)
{
	if (m_attackReady)
	{
		m_attackManager.startAttack();
		m_attackReady = false;
	}
}

void Rat::handleMovement()
{
	switch (m_currentDirection)
	{
	case (Direction::Right) :
	{
		move(Direction::Right);
		break;
	}
	case (Direction::Left) : 
	{
		move(Direction::Left);
		break;
	}
	}
}