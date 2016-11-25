#include "Managers\AttackManager.h"
#include "Entities\Character.h"
#include "Entities\EntityState.h"
#include "SpriteSheet.h"
#include "Managers\EntityManager.h"
#include "Game\Map.h"

AttackManager::AttackManager(Character* const entity, const SharedContext& sharedContext)
	: m_character(entity),
	m_sharedContext(sharedContext)
{
	const int tileSize = m_sharedContext.m_map->getTileSize();
	m_attackAABB.width = tileSize;
	m_attackAABB.height = tileSize;
}

AttackManager::~AttackManager()
{
}

void AttackManager::startAttack()
{
	//Do not attack in one of these states
	const EntityState state = m_character->getState();
	if (state == EntityState::Dead ||
		state == EntityState::Attacking ||
		state == EntityState::Hurt)
	{
		return;
	}

	attackPreparation();	
}

void AttackManager::attackPreparation()
{
	//Begin attack
	m_character->setState(EntityState::Attacking);
	m_character->m_spriteSheet.setAnimationType(AnimationName::Attack, m_character->getDirection());
	m_character->m_audioPlayer.play("Attack", false);
	m_character->setVelocity(0, m_character->getVelocity().y);
	const int tileSize = m_character->m_spriteSheet.getTileSize();
	m_attackAABB.top = m_character->getAABB().top;
	const int y = m_attackAABB.top / tileSize;
	int fromX = 0;
	int toX = 0;

	//Get attack information from specific direction
	switch (m_character->getDirection())
	{
	case (Direction::Right):
	{
		m_attackAABB.left = m_character->getAABB().left;

		fromX = std::floor((m_attackAABB.left) / tileSize);
		toX = std::floor((m_attackAABB.left + (m_attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	case (Direction::Left):
	{
		m_attackAABB.left = m_character->getAABB().left;

		toX = std::floor(m_attackAABB.left / tileSize);
		fromX = std::floor((m_attackAABB.left - (m_attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	}

	for (int x = fromX; x < toX; ++x)
	{
		Entity* const entity = m_sharedContext.m_entityManager->getEntityAtPosition(sf::Vector2i(x, y));
		if (entity)
		{
			m_entitiesAtPos.emplace_back(std::make_pair(sf::Vector2i(x, y), entity));
		}
	}

	for (const auto &i : m_entitiesAtPos)
	{
		//Do not allow to attack the same Entity
		if (i.second->getID() != m_character->getID())
		{
			attack(i.first, *i.second);
		}
	}
}

void AttackManager::attack(const sf::Vector2i & attackPos, Entity & entity)
{
	const int tileSize = m_character->m_spriteSheet.getTileSize();
	const sf::FloatRect entityAABB = entity.getAABB();
	const sf::FloatRect attackBounds(attackPos.x * tileSize, attackPos.y * tileSize, tileSize, tileSize);
	if (attackBounds.intersects(entityAABB))
	{
		entity.onEntityCollision(entity);
	}
}
