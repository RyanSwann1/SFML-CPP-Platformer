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
	friend class AttackManager;
public:
	Entity(const SharedContext& sharedContext, const std::string& name);
	virtual ~Entity();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	const EntityType getType() const { return m_type; }
	const sf::Vector2i getPosition() const;
	const sf::Vector2f& getActualPosition() const { return m_position; }
	const int getID() const { return m_ID; }
	const sf::FloatRect& getAABB() const { return m_AABB; }

	inline void draw(sf::RenderWindow& window) { m_spriteSheet.draw(window); }
	
protected:
	inline void setPosition(const sf::Vector2f& pos) { m_position = pos; }
	inline void setMaxVelocity(const sf::Vector2f& maxVel) { m_maxVelocity = maxVel; }
	inline void setSpeed(const sf::Vector2f& speed) { m_speed = speed; }
	inline void setType(const EntityType type) { m_type = type; }
	inline void setDirection(const Direction dir) { m_currentDirection = dir; }

	inline const sf::Vector2f& getSpeed() const { return m_speed; }
	inline const sf::Vector2f& getVelocity() const { return m_velocity; }
	inline const std::string& getName() const { return m_name; }

	inline const EntityType getEntityType() const { return m_type; }
	inline SpriteSheet& getSpriteSheet() { return m_spriteSheet; }
	inline SharedContext& getSharedContext() { return m_sharedContext; }
	inline AudioPlayer& getAudioPlayer() { return m_audioPlayer; }
	inline CollisionManager& getCollisionManager() { return m_collisionManager; }
	inline void stop() { m_velocity.x = 0; }
	inline void setAcceleration(const float x, const float y) { m_acceleration = sf::Vector2f(x, y); }
	inline void accelerate(const float x, const float y) { m_acceleration += sf::Vector2f(x, y); }
	inline const Direction getDirection() const { return m_currentDirection; }
	inline const Tile* const getReferenceTile() const { return m_referenceTile; }
	inline const bool isCollidingOnX() const { return m_collidingOnX; }
	inline const bool isCollidingOnY() const { return m_collidingOnY; }
	void addVelocity(const float x, const float y);
	
	void setVelocity(const float x, const float y);
	
	virtual void moveInDirection(const Direction dir);
	virtual void update(const float deltaTime);
	virtual void resolveCollisions(std::vector<CollisionElement*>& collisions);


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
	virtual void onEntityCollision(Entity& entity) = 0;
	
	Direction m_currentDirection;
	EntityType m_type;
	SpriteSheet m_spriteSheet;
	SharedContext m_sharedContext;
	AudioPlayer m_audioPlayer;
	CollisionManager m_collisionManager;
	const Tile* m_referenceTile; //Tile that the entity is currently standing on
	bool m_collidingOnX;
	bool m_collidingOnY;

	inline void setInitialPosition(const sf::Vector2f& initialPos)
	{
		m_position = initialPos;
		m_startPosition = initialPos;
	}
	void move(const float x, const float y);
	void applyFriction(const float deltaTime);
	void applyGravity();
	void updateAABB();
	void remove(); //Remove this entity from the game
	inline void setID(const int ID)
	{
		if (ID >= 0)
		{
			m_ID = ID;
		}
	}
	//inline bool isCollidingOnX() { return m_collidingOnX; }
	//inline bool isCollidingOnY() { return m_collidingOnY; }
	//inline void setCollidingOnX() { m_collidingOnX = true; }
	//inline void setCollidingOnY() { m_collidingOnY = true; }

	inline void resetCollisions()
	{
		m_collidingOnX = false;
		m_collidingOnY = false;
	}
};

