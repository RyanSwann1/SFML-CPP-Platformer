#pragma once

#include "Entities\Entity.h"
#include "Game\Timer.h"
#include "Utilities.h"
#include "SharedContext.h"
#include <functional>
#include <SFML\Graphics.hpp>

struct SharedContext;

template <class T>
class EntitySpawner
{
public:
	//Is it good practice to add something like this so I dont have to make the register entity function public? 
	friend class StateGame;

	EntitySpawner(SharedContext* sharedContext)
		: m_sharedContext(sharedContext),
		m_minSpawnPos(25),
		m_maxSpawnPos(550),
		m_spawnReady(true)
	{
		registerEntity(sharedContext);
		m_spawnTimer.setExpirationTime(4);
		m_spawnTimer.activate();
	}
	~EntitySpawner() {};

	EntitySpawner(const EntitySpawner&) = delete;
	EntitySpawner&(const EntitySpawner&) = delete;

	void update(const float deltaTime)
	{
		if (m_spawnReady)
		{
			spawnEntity();
			m_spawnReady = false;
		}

		m_spawnTimer.update(deltaTime);
		if (m_spawnTimer.isExpired())
		{
			m_spawnTimer.reset();
			m_spawnReady = true;
		}
	}

private:
	std::function<Entity*()> m_entityToSpawn;
	SharedContext* m_sharedContext;
	Timer m_spawnTimer;
	bool m_spawnReady;
	int m_minSpawnPos;
	int m_maxSpawnPos;

	void spawnEntity()
	{
		EntityManager* entityManager = m_sharedContext->m_entityManager;
		Entity* entity = m_entityToSpawn();
		entity->setPosition(getSpawnPos());
		entityManager->add(entity);
	}

	sf::Vector2f getSpawnPos()
	{
		return sf::Vector2f(Utilities::randomNumberGenerator(m_minSpawnPos, m_maxSpawnPos), 420);
	}

	void registerEntity(SharedContext* sharedContext)
	{
		m_entityToSpawn = [sharedContext]()-> Entity*
		{
			return new T(sharedContext);
		};
	}
};