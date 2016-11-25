#pragma once

#include "SharedContext.h"
#include "Entities\Entity.h"
#include <vector>
#include <SFML\Graphics.hpp>

class Character;
class AttackManager
{
public:
	AttackManager(Character* const entity, const SharedContext& sharedContext);
	~AttackManager();

	void startAttack();
	
private:
	Character* const m_character;
	SharedContext m_sharedContext;
	std::vector<std::pair<sf::Vector2i, Entity*>> m_entitiesAtPos;

	void attackPreparation();
	void attack(const sf::Vector2i& attackPos, Entity& entity);
	sf::FloatRect m_attackAABB;
};

