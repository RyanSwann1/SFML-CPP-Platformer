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
	m_name(name),
	m_collidingOnX(false),
	m_collidingOnY(false),
	m_referenceTile(nullptr)
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

void Entity::remove()
{
	m_sharedContext.m_entityManager->removeEntity(getID());
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
	
	m_collisionManager.update(this);
}

void Entity::resolveCollisions(std::vector<CollisionElement*>& collisions)
{
	if (collisions.empty()) {
		return;
	}

	//Sort so collisions are in greatest area order
	std::sort(collisions.begin(), collisions.end(), [](CollisionElement* col1, CollisionElement* col2) { return col1->m_area > col2->m_area; });

	for (const auto &i : collisions)
	{
		const sf::FloatRect AABB(getAABB());
		//Make sure that entity is still colliding with the collision box in question
		if (!AABB.intersects(i->m_collisionBox))
		{
			continue;
		}

		const sf::FloatRect collisionBox(i->m_collisionBox);
		const float xDifference = (AABB.left + (AABB.width / 2.0f)) - (collisionBox.left + (collisionBox.width / 2.0f));
		const float yDifference = (AABB.top + (AABB.height / 2.0f)) - (collisionBox.top + (collisionBox.height / 2.0f));
		float resolve = 0;

		const sf::FloatRect intersection(i->m_intersection);
		if (std::abs(xDifference) > std::abs(yDifference))
		{
			if (xDifference > 0) {
				resolve = intersection.width;
			}
			else {
				resolve = -intersection.width;
			}
			move(resolve, 0);
			m_velocity.x = 0;
			m_collidingOnX = true;
		}
		else
		{
			if (yDifference > 0) {
				resolve = intersection.height;
			}
			else {
				resolve = -intersection.height;
			}
			move(0, resolve);
			m_velocity.y = 0;
			m_collidingOnY = true;
		}
		//Assign the reference tile for the entity
		if (m_collidingOnY)
		{
			//m_collisionBoxes.push_back(CollisionBox(sf::Vector2f(collisionBox.left, collisionBox.top)));
			const Map& map = *m_sharedContext.m_map;
			const Tile* const tile = map.getTile(std::floor(collisionBox.left / Sheet::TILE_SIZE), std::floor(collisionBox.top / Sheet::TILE_SIZE));
			if (tile)
			{
				m_referenceTile = tile;
			}
		}
	}
}