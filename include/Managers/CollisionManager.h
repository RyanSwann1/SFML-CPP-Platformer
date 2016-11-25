#pragma once

#include "Direction.h"
#include "Entities\EntityType.h"
#include "SharedContext.h"
#include <SFML\Graphics.hpp>
#include <vector>

class Entity;
struct Tile;
struct SharedContext;
struct CollisionElement
{
	CollisionElement(const float area, const sf::FloatRect& intersection, const sf::FloatRect& collisionBox)
		: m_area(area),
		m_intersection(intersection),
		m_collisionBox(collisionBox)
	{}

	const float m_area;
	const sf::FloatRect m_intersection;
	const sf::FloatRect m_collisionBox;
};

class CollisionManager
{
public:
	CollisionManager(const SharedContext& sharedContext, Entity& entity);
	~CollisionManager();

	CollisionManager(const CollisionManager&) = delete;
	CollisionElement& operator=(const CollisionManager&) = delete;

	const bool collidingOnXAxis() const { return m_collidingOnX; }
	const bool collidingOnYAxis() const { return m_collidingOnY; }
	inline void resetCollisionChecks()
	{
		m_collidingOnX = false;
		m_collidingOnY = false;
	}
	
	void update();
	const Tile* getReferenceTile() const { return m_referenceTile; }

private:
	SharedContext m_sharedContext;
	const Tile* m_referenceTile; //Tile that entity is currently standing on
	Entity* const m_entity;
	std::vector<CollisionElement*> m_collisions;
	bool m_collidingOnY;
	bool m_collidingOnX;
	
	void checkEntityCollisions();
	void checkTileCollisions();
	void resolveCollisions();
};