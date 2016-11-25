#include "Managers\CollisionManager.h"
#include "Game\Map.h"
#include "Entities\Entity.h"
#include "Managers\EntityManager.h"
#include <math.h>
#include <iostream>

CollisionManager::CollisionManager(const SharedContext& sharedContext, Entity& entity)
	: m_sharedContext(sharedContext),
	m_entity(&entity),
	m_referenceTile(nullptr),
	m_collidingOnX(false),
	m_collidingOnY(false)
{}

CollisionManager::~CollisionManager()
{}

void CollisionManager::update()
{
	checkTileCollisions();
	resolveCollisions();
	checkEntityCollisions();

	m_collisions.clear();

	if (!m_collidingOnY)
	{
		m_referenceTile = nullptr;
	}
}

void CollisionManager::checkEntityCollisions()
{
	const EntityManager& entityManager = *m_sharedContext.m_entityManager;
	Entity* const entity2 = entityManager.getEntityAtPosition(m_entity->getPosition());
	if (entity2)
	{
		m_entity->onEntityCollision(*entity2);
		entity2->onEntityCollision(*m_entity);
	}
}

void CollisionManager::checkTileCollisions()
{
	const int tileSize = Sheet::TILE_SIZE;
	const Map& map = *m_sharedContext.m_map;
	const sf::FloatRect AABB = m_entity->getAABB();

	const int fromX = std::floor((AABB.left - (AABB.width)) / tileSize);
	const int toX = std::floor((AABB.left + (AABB.width * 2)) / tileSize);
	const int fromY = std::floor((AABB.top - (AABB.height)) / tileSize);
	const int toY = std::floor((AABB.top + (AABB.height * 2)) / tileSize);

	for (int y = fromY; y < toY; ++y)
	{
		for (int x = fromX; x < toX; ++x)
		{
			const Tile* tile = map.getTile(x, y);
			if (tile)
			{
				const sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);

				sf::FloatRect intersection;
				if (AABB.intersects(tileBounds, intersection))
				{
					const float area = intersection.height * intersection.width;
					m_collisions.push_back(new CollisionElement(area, intersection, tileBounds));
				}
			}
		}
	}
}

void CollisionManager::resolveCollisions()
{
	if (m_collisions.empty()) {
		return;
	}

	//Sort so collisions are in greatest area order
	std::sort(m_collisions.begin(), m_collisions.end(), [](CollisionElement* col1, CollisionElement* col2) { return col1->m_area > col2->m_area; });

	for (const auto &i : m_collisions)
	{
		sf::FloatRect AABB = m_entity->getAABB();
		//Make sure that entity is still colliding with the collision box in question
		if (!AABB.intersects(i->m_collisionBox))
		{
			continue;
		}
		
		const sf::FloatRect collisionBox = i->m_collisionBox;
		const float xDifference = (AABB.left + (AABB.width / 2.0f)) - (collisionBox.left + (collisionBox.width / 2.0f));
		const float yDifference = (AABB.top + (AABB.height / 2.0f)) - (collisionBox.top + (collisionBox.height / 2.0f));
		float resolve = 0;

		const sf::FloatRect intersection = i->m_intersection;
		if (std::abs(xDifference) > std::abs(yDifference))
		{
			if (xDifference > 0) {
				resolve = intersection.width;
			}
			else {
				resolve = -intersection.width;
			}
			m_entity->move(resolve, 0);
			m_entity->m_velocity.x = 0;
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
			m_entity->move(0, resolve);
			m_entity->m_velocity.y = 0;
			m_collidingOnY = true;
		}
		//Assign the reference tile for the entity
		if (m_collidingOnY)
		{
			//m_collisionBoxes.push_back(CollisionBox(sf::Vector2f(collisionBox.left, collisionBox.top)));
			const Map& map = *m_sharedContext.m_map;
			const Tile* tile = map.getTile(std::floor(collisionBox.left / Sheet::TILE_SIZE), std::floor(collisionBox.top / Sheet::TILE_SIZE));
			if (tile)
			{
				m_referenceTile = tile;
			}
		}
	}
	m_collisions.clear();
}