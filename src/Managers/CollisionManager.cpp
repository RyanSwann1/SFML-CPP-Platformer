#include "Managers\CollisionManager.h"
#include "Game\Map.h"
#include "Entities\Entity.h"
#include "Managers\EntityManager.h"
#include <math.h>
#include <iostream>
#include <SFML\Graphics.hpp>

CollisionManager::CollisionManager(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext)
{}

CollisionManager::~CollisionManager()
{}

void CollisionManager::update(Entity* const entity)
{
	handleTileCollisions(entity);
	handleEntityCollisions(entity);

	if (!entity->m_collidingOnY)
	{
		entity->m_referenceTile = nullptr;
	}

	entity->resetCollisions();
}

void CollisionManager::checkEntityCollisions(Entity* const entity)
{
	const EntityManager& entityManager = *m_sharedContext.m_entityManager;
	Entity* const entity2 = entityManager.getEntityAtPosition(entity->getPosition());
	if (entity2)
	{
		entity->onEntityCollision(*entity2);
		entity2->onEntityCollision(*entity);
	}
}

void CollisionManager::checkTileCollisions(const Entity* const entity, std::vector<CollisionElement*>& collisions)
{
	const int tileSize = Sheet::TILE_SIZE;
	const Map& map = *m_sharedContext.m_map;
	const sf::FloatRect AABB(entity->getAABB());

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
					collisions.push_back(new CollisionElement(area, intersection, tileBounds));
				}
			}
		}
	}
}

void CollisionManager::handleTileCollisions(Entity* const entity)
{
	std::vector<CollisionElement*> collisions;
	checkTileCollisions(entity, collisions);
	entity->resolveCollisions(collisions);
	collisions.clear();
}

void CollisionManager::handleEntityCollisions(Entity* const entity)
{
	checkEntityCollisions(entity);
}