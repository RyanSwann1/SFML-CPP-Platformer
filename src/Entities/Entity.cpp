#include "Entities\Entity.h"
#include "Game\Map.h"
#include "SharedContext.h"
#include "Managers\TextureManager.h"
#include "Managers\EntityManager.h"
#include <math.h>
#include <algorithm>

Entity::Entity(const SharedContext& sharedContext, const std::string& name)
	: m_sharedContext(sharedContext),
	m_spriteSheet(*&sharedContext),
	m_audioPlayer(sharedContext),
	m_collisionManager(*&sharedContext, *this),
	m_ID(0),
	m_name(name)
{
	m_friction = sf::Vector2f(0.075f, 0);
	m_gravity = sf::Vector2f(0, 500);

	//m_gravity = sharedContext->m_map->getDefaultGravity();
	//m_friction = sharedContext->m_map->getDefaultFriction();
}

Entity::~Entity()
{
	TextureManager& textureManager = *m_sharedContext.m_textureManager;
	textureManager.releaseResource(m_spriteSheet.getName());
}

void Entity::setVelocity(const float x, const float y)
{
	m_velocity = sf::Vector2f(x, y);
	//if (std::abs(x) <= m_maxVelocity.x && std::abs(y) <= m_maxVelocity.y)
	//{
	//	
	//}
}

const sf::Vector2i Entity::getPosition() const
{
	const int x = std::floor(m_AABB.left / Sheet::TILE_SIZE);
	const int y = std::floor(m_AABB.top / Sheet::TILE_SIZE);
	return sf::Vector2i(x, y);
}

void Entity::addVelocity(const float x, const float y)
{
	m_velocity += sf::Vector2f(x, y);
	
	//Keep m_velocity within confines of the maximum velocity
	if (std::abs(m_velocity.x) > m_maxVelocity.x)
	{
		if (m_velocity.x > 0.0f)
		{
			m_velocity.x = m_maxVelocity.x;
		}
		else 
		{
			m_velocity.x = -m_maxVelocity.x;
		}
	}

	if (std::abs(m_velocity.y) > m_maxVelocity.y)
	{
		if (m_velocity.y > 0.0f)
		{
			m_velocity.y = m_maxVelocity.y;
		}
		else
		{
			m_velocity.y = -m_maxVelocity.y;
		}
	}
}

void Entity::move(const float x, const float y)
{
	m_oldPosition = m_position;
	m_position += sf::Vector2f(x, y);

	//Check for map boundaries
	const sf::Vector2u maxMapSize = m_sharedContext.m_map->getMaxMapSize();
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}
	updateAABB();

	//After movement reset velocity
	//m_velocity = sf::Vector2f(0, 0);
}

void Entity::applyFriction(const float deltaTime)
{
	//Apply friction
	if ((std::abs(m_velocity.x) - std::abs(m_friction.x)) > 0.1f)
	{
		if (m_velocity.x > 0.1f)
		{
			m_velocity.x -= m_friction.x;
		}
		else
		{
			m_velocity.x += m_friction.x;
		}
		
	}
	//https://www.youtube.com/watch?v=lz0YwHFpo-A
}

void Entity::applyGravity()
{
	accelerate(0, m_gravity.y);
}

void Entity::updateAABB()
{
	m_AABB.left = m_position.x;
	m_AABB.top = m_position.y;
	m_AABB.width = Sheet::TILE_SIZE;
	m_AABB.height = Sheet::TILE_SIZE;
}

void Entity::update(const float deltaTime)
{
	applyGravity();
	applyFriction(deltaTime);

	addVelocity(m_acceleration.x * deltaTime, m_acceleration.y * deltaTime);
	setAcceleration(0, 0);

	//Move entity
	const sf::Vector2f deltaPos = m_velocity * deltaTime;
	move(deltaPos.x, deltaPos.y);
	
	m_collisionManager.update();
	resolveCollisions();

	//Called here to allow entity to resolve impact of collisions
	m_collisionManager.resetCollisionChecks();
}

void Entity::draw(sf::RenderWindow & window)
{
	m_spriteSheet.draw(window);
}