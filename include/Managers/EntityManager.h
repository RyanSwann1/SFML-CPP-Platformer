#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <list>
#include <SFML\Graphics.hpp>

class Entity;
class EntityManager
{
	friend class Map;
public:
	EntityManager();
	~EntityManager();

	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	void add(Entity& entity);
	void add(Entity& entity, const sf::Vector2f& pos);
	void removeEntity(const unsigned int id);

	Entity* findEntity(const std::string& name) const;
	Entity* getEntityAtPosition(const sf::Vector2i& pos) const;
	const std::string getEntityTypeLocation(const std::string& name) const;

	void update(const float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<std::string, std::string> m_entityTypes;
	std::list<std::pair<unsigned int, Entity*>> m_entities;
	std::vector<unsigned int> m_removals;

	unsigned int m_entityID;
	void processRemovals();
	void removeActiveEntity(unsigned int entityID);
	void loadInEntityTypes(const std::string& fileName);
	void purgeEntities();
};

