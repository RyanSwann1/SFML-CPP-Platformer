#pragma once

#include "Entities\EntityType.h"
#include "Direction.h"
#include "SpriteSheet.h"
#include "Audio\AudioPlayer.h"
#include "Managers\CollisionManager.h"
#include "SharedContext.h"
#include <unordered_map>
#include <vector>
#include <SFML\Graphics.hpp>
#include <string>

struct Tile;
struct TileInfo;
struct SharedContext;

class Entity
{
	friend class EntityManager;
	friend class CollisionManager;
public:
	Entity(const SharedContext& sharedContext, const std::string& name);
	virtual ~Entity();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	void setVelocity(const float x, const float y);

	void setAcceleration(const float x, const float y) { m_acceleration = sf::Vector2f(x, y); }
	//void setPosition(const sf::Vector2f& pos) { m_position = pos; }
	inline void setID(const int ID)
	{
		if (ID >= 0)
		{
			m_ID = ID;
		}
	}
	inline void setInitialPosition(const sf::Vector2f& initialPos)
	{
		m_position = initialPos;
		m_startPosition = initialPos;
	}

	const EntityType getType() const { return m_type; }
	const sf::Vector2i getPosition() const;
	const sf::Vector2f& getActualPosition() const { return m_position; }

	const int getID() const { return m_ID; }
	const sf::FloatRect& getAABB() const { return m_AABB; }
	const Direction getDirection() const { return m_currentDirection; }
	const Tile* const getReferenceTile() const { return m_collisionManager.getReferenceTile(); }

	void addVelocity(const float x, const float y);
	void move(const float x, const float y);
	void accelerate(const float x, const float y) { m_acceleration += sf::Vector2f(x, y); }
	void applyFriction(const float deltaTime);
	void applyGravity();
	void updateAABB();
	void stop() { m_velocity.x = 0; }

	virtual void update(const float deltaTime);
	virtual void onEntityCollision(Entity& entity) = 0;
	void draw(sf::RenderWindow& window);

protected:
	Direction m_currentDirection;
	EntityType m_type;
	SpriteSheet m_spriteSheet;
	SharedContext m_sharedContext;
	AudioPlayer m_audioPlayer;
	CollisionManager m_collisionManager;

	virtual void resolveCollisions() = 0;

	void setPosition(const sf::Vector2f& pos) { m_position = pos; }
	void setMaxVelocity(const sf::Vector2f& maxVel) { m_maxVelocity = maxVel; }
	void setSpeed(const sf::Vector2f& speed) { m_speed = speed; }

	const sf::Vector2f& getSpeed() const { return m_speed; }
	const sf::Vector2f& getVelocity() const { return m_velocity; }
	const std::string& getName() const { return m_name; }

private:
	int m_ID;
	const std::string m_name;
	sf::Vector2f m_friction;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_gravity;
	sf::Vector2f m_maxVelocity;
	sf::Vector2f m_oldPosition;
	sf::Vector2f m_startPosition;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	sf::Vector2f m_velocity;
	sf::FloatRect m_AABB;
};

