#include "Managers\AttackManager.h"
#include "Entities\Character.h"
#include "Entities\EntityState.h"
#include "SpriteSheet.h"
#include "Managers\EntityManager.h"
#include "Game\Map.h"
#include "Animation\AnimationName.h"


AttackManager::AttackManager(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext)
{}

AttackManager::~AttackManager()
{}

void AttackManager::startAttack(Character& character)
{
	if (!proceedWithAttack(character)) 
	{
		return;
	}

	
	std::vector<std::pair<sf::Vector2f, Entity*>> entitiesToAttack = attackPreparation(character);

	for (const auto &i : entitiesToAttack)
	{
		//Do not allow to attack the same Entity
		if (i.second->getID() != character.getID())
		{
			attack(character, i.first, *i.second);
		}
	}

	entitiesToAttack.clear();
}

std::vector<std::pair<sf::Vector2f, Entity*>> AttackManager::attackPreparation(Character& character)
{
	//Begin attack
	const int tileSize = Sheet::TILE_SIZE;
	sf::FloatRect attackAABB = character.m_attackAABB;
	attackAABB.top = character.getAABB().top;
	const int y = attackAABB.top / tileSize;
	int fromX = 0;
	int toX = 0;

	//Get attack information from specific direction
	switch (character.getDirection())
	{
	case (Direction::Right):
	{
		attackAABB.left = character.getAABB().left;

		fromX = std::floor((attackAABB.left) / tileSize);
		toX = std::floor((attackAABB.left + (attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	case (Direction::Left):
	{
		attackAABB.left = character.getAABB().left;

		toX = std::floor(attackAABB.left / tileSize);
		fromX = std::floor((attackAABB.left - (attackAABB.width * 2.0f)) / tileSize);
		break;
	}
	}

	std::vector<std::pair<sf::Vector2f, Entity*>> entitiesToAttack;
	for (int x = fromX; x < toX; ++x)
	{
		Entity* const entity = m_sharedContext.m_entityManager->getEntityAtPosition(sf::Vector2i(x, y));
		if (entity)
		{
			entitiesToAttack.emplace_back(std::make_pair(sf::Vector2i(x, y), entity));
		}
	}
	return entitiesToAttack;
}

void AttackManager::attack(Character& character, const sf::Vector2f & attackPos, Entity & entityToAttack)
{
	const int tileSize = Sheet::TILE_SIZE;
	const sf::FloatRect entityAABB = entityToAttack.getAABB();
	const sf::FloatRect attackBounds(attackPos.x * tileSize, attackPos.y * tileSize, tileSize, tileSize);
	if (attackBounds.intersects(entityAABB))
	{
		entityToAttack.onEntityCollision(dynamic_cast<Entity&>(character));
	}
}

const bool AttackManager::proceedWithAttack(Character & character) const
{
	if (character.getState() == EntityState::Dead ||
		character.getState() == EntityState::Hurt ||
		character.getState() == EntityState::Attacking)
	{
		return false;
	}
	return true;
}
