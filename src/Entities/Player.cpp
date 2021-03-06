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
	setType(EntityType::Player);
	Character::load(getSharedContext().m_entityManager->getEntityTypeLocation(name));
	Entity::setDirection(Direction::Right);

	AudioPlayer& audioPlayer = getAudioPlayer();
	audioPlayer.registerAudioClip(name, "Attack");
	audioPlayer.registerAudioClip(name, "Jump");
	audioPlayer.registerAudioClip(name, "Death");
	audioPlayer.registerAudioClip(name, "Hurt");
	audioPlayer.registerOwner(name);

	EventManager& eventManager = *getSharedContext().m_eventManager;
	eventManager.addCallback<Player>(KeyBinding::Move_Left, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Move_Right, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Jump, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Stop, StateType::Game, &Player::reactToInput, this);
	eventManager.addCallback<Player>(KeyBinding::Action_Button, StateType::Game, &Player::reactToInput, this);
}

Player::~Player()
{
	EventManager& eventManager = *getSharedContext().m_eventManager;
	eventManager.removeCallback(KeyBinding::Move_Left);
	eventManager.removeCallback(KeyBinding::Move_Right);
	eventManager.removeCallback(KeyBinding::Jump);
	eventManager.removeCallback(KeyBinding::Action_Button);
	eventManager.removeCallback(KeyBinding::Stop);

	const std::string name(Entity::getName());
	AudioPlayer& audioPlayer = getAudioPlayer();
	audioPlayer.removeAudioClip(name, "Attack");
	audioPlayer.removeAudioClip(name, "Jump");
	audioPlayer.removeAudioClip(name, "Death");
	audioPlayer.removeAudioClip(name, "Hurt");
	audioPlayer.removeOwner(name);
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
	switch (eventDetails.m_keyBinding)
	{
	case (KeyBinding::Move_Left) :
	{
		Character::moveInDirection(Direction::Left);
		break;
	}
	case (KeyBinding::Move_Right) : 
	{
		Character::moveInDirection(Direction::Right);
		break;
	}
	case (KeyBinding::Action_Button) :
	{
		Character::attack();
		break;
	}
	case (KeyBinding::Stop) :
	{
		Entity::stop();
		break;
	}
	case (KeyBinding::Jump) :
	{
		Character::jump();
		break;
	}
	}
}

void Player::resolveCollisions(std::vector<CollisionElement*>& collisions)
{
	Character::resolveCollisions(collisions);

	if (getReferenceTile())
	{
		//Standing on dedadly tile
		if (getReferenceTile()->m_deadly) 
		{
			killCharacter();
		}
		//Standing on tile that'll change level
		else if (getReferenceTile()->m_warp) 
		{
			Map& map = *getSharedContext().m_map;
			map.loadNextLevel();
		}
	}
}