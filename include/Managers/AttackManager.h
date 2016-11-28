#pragma once

#include "SharedContext.h"
#include <vector>
#include <SFML\Graphics.hpp>

class Entity;
class Character;
class AttackManager
{
public:
	AttackManager(const SharedContext& sharedContext);
	~AttackManager();

	void startAttack(Character& character);
	
private:
	SharedContext m_sharedContext;

	std::vector<std::pair<sf::Vector2f, Entity*>> attackPreparation(Character& character);
	void attack(Character& character, const sf::Vector2f& attackPos, Entity& entityToAttack);

	const bool proceedWithAttack(Character& character) const;
};

