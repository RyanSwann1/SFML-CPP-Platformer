#include "Entities\Computer.h"
#include "SharedContext.h"
#include "Game\Map.h"
#include <iostream>
Computer::Computer(const SharedContext& sharedContext, const std::string& name)
	: Character(sharedContext, name)
{
}

Computer::~Computer()
{
}

void Computer::update(const float deltaTime)
{
	Character::update(deltaTime);
}

void Computer::resolveCollisions()
{
	Character::resolveCollisions();

	if (m_collisionManager.collidingOnXAxis())
	{
		changeDirection();
	}
	const Tile* const tile = m_collisionManager.getReferenceTile();
	if (tile)
	{
		if (tile->m_deadly)
		{
			changeDirection();
		}
	}
}
