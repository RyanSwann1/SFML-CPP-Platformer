#include "Entities\Rat.h"
#include "SharedContext.h"
#include "Managers\EntityManager.h"
#include "Utilities.h"
#include "Game\Map.h"

Rat::Rat(const SharedContext& sharedContext, const std::string& name)
	: Character(sharedContext, name),
	m_attackReady(true)
{
	setType(EntityType::Enemy);
	setDirection(static_cast<Direction>(Utilities::randomNumberGenerator(0, 1)));
	getAudioPlayer().registerOwner(name);
	Character::load(getSharedContext().m_entityManager->getEntityTypeLocation(name));
	m_attackTimer.setExpirationTime(4);
	m_attackTimer.activate();
	Entity::setPosition(sf::Vector2f(400, 400));
}

Rat::~Rat()
{}

void Rat::update(const float deltaTime)
{
	Character::update(deltaTime);
	Character::moveInDirection(Entity::getDirection());
	
	if (m_attackReady)
	{
		Character::attack();
		m_attackReady = false;
	}

	handleAttackTimer(deltaTime);
}

void Rat::handleAttackTimer(const float deltaTime)
{
	m_attackTimer.update(deltaTime);
	if (m_attackTimer.isExpired())
	{
		m_attackTimer.resetTime();
		m_attackReady = true;
	}
}

void Rat::onEntityCollision(Entity& entity)
{
	if (Character::getState() != EntityState::Dead)
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

void Rat::resolveCollisions(std::vector<CollisionElement*>& collisions)
{
	Character::resolveCollisions(collisions);

	if (isCollidingOnX())
	{
		switch (Entity::getDirection())
		{
		case (Direction::Left) :
		{
			Character::moveInDirection(Direction::Right);
			break;
		}
		case (Direction::Right) :
		{
			Character::moveInDirection(Direction::Left);
			break;
		}
		}
	}
}