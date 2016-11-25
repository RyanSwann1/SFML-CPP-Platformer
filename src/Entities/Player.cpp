#include "Entities\Player.h"
#include "Managers\EventManager.h"
#include "SharedContext.h"
#include "Managers\EntityManager.h"
#include "Managers\AudioManager.h"
#include "Managers\StateManager.h"
#include "Game\Map.h"
#include "States\StateType.h"
#include <iostream>

Player::Player(SharedContext& sharedContext, const std::string& name)
	: Character(sharedContext, name)
{
	m_type = EntityType::Player;
	Character::load(m_sharedContext.m_entityManager->getEntityTypeLocation(name));
	setDirection(Direction::Right);

	m_audioPlayer.registerAudioClip(name, "Attack");
	m_audioPlayer.registerAudioClip(name, "Jump");
	m_audioPlayer.registerAudioClip(name, "Death");
	m_audioPlayer.registerAudioClip(name, "Hurt");
	m_audioPlayer.registerOwner(name);

	EventManager& eventManager = *m_sharedContext.m_eventManager;
	eventManager.addCallback<Player>(KeyBinding::Move_Left, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Move_Right, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Jump, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Stop, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Action_Button, StateType::Game, &Player::reactToInput, this);
}

Player::~Player()
{
	EventManager& eventManager = *m_sharedContext.m_eventManager;
	eventManager.removeCallback(KeyBinding::Move_Left);
	eventManager.removeCallback(KeyBinding::Move_Right);
	eventManager.removeCallback(KeyBinding::Jump);
	eventManager.removeCallback(KeyBinding::Action_Button);
	eventManager.removeCallback(KeyBinding::Stop);

	std::string name = Entity::getName();
	m_audioPlayer.removeAudioClip(name, "Attack");
	m_audioPlayer.removeAudioClip(name, "Jump");
	m_audioPlayer.removeAudioClip(name, "Death");
	m_audioPlayer.removeAudioClip(name, "Hurt");
	m_audioPlayer.removeOwner(name);
}

void Player::onEntityCollision(Entity& entity)
{
	if (entity.getType() == EntityType::Enemy)
	{
		const Character& character = static_cast<Character&>(entity);
		reduceLife(character.getDamageValue());
	}
}

void Player::reactToInput(const EventDetails& eventDetails)
{
	if (eventDetails.m_keyBinding == KeyBinding::Move_Left)
	{
		move(Direction::Left);
	}
	else if (eventDetails.m_keyBinding == KeyBinding::Move_Right)
	{
		move(Direction::Right);
	}
	else if (eventDetails.m_keyBinding == KeyBinding::Action_Button)
	{
		m_attackManager.startAttack();
	}
	else if (eventDetails.m_keyBinding == KeyBinding::Jump)
	{
		jump();
	}
	else if (eventDetails.m_keyBinding == KeyBinding::Stop)
	{
		Entity::setVelocity(0, Entity::getVelocity().y);
	}
}

void Player::resolveCollisions()
{
	Character::resolveCollisions();

	const Tile* const referenceTile = m_collisionManager.getReferenceTile();
	if (referenceTile)
	{
		//Standing on dedadly tile
		if (referenceTile->m_deadly) 
		{
			killCharacter();
		}
		//Standing on tile that'll change level
		else if (referenceTile->m_warp) 
		{
			Map& map = *m_sharedContext.m_map;
			map.loadNextLevel();
		}
	}
}