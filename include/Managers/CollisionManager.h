#pragma once

#include "SharedContext.h"
#include <vector>
#include <SFML\Graphics.hpp>

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
	CollisionManager& operator=(const CollisionManager&) = delete;
	
	void update(Entity* const entity);
	
private:
	SharedContext m_sharedContext;
	
	void checkEntityCollisions(Entity* const entity);
	void checkTileCollisions(const Entity* const entity, std::vector<CollisionElement*>& collisions);
	
	void handleTileCollisions(Entity* const entity);
	void handleEntityCollisions(Entity* const entity);
};