#pragma once

#include "Managers\TextureManager.h"
#include "SharedContext.h"
#include <SFML\Graphics.hpp>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <array>

enum class MapNames
{
	Map1 = 0,
	Map2,
	Total 
};

struct Sheet
{
	static const int TILE_SIZE = 32;
	static const unsigned int WIDTH = 256;
	static const unsigned int HEIGHT = 256;
};

class Entity;
struct TileInfo
{
	TileInfo(TextureManager& textureManager, const std::string& tileSheetName, const unsigned int ID)
		: m_ID(ID),
		m_tileSheetName(tileSheetName),
		m_textureManager(&textureManager)

	{
		m_sprite.setTexture(*m_textureManager->getResource(tileSheetName));

		sf::IntRect tileBoundaries;
		tileBoundaries.left = m_ID % (Sheet::WIDTH / Sheet::TILE_SIZE) * Sheet::TILE_SIZE;
		tileBoundaries.top = m_ID / (Sheet::HEIGHT / Sheet::TILE_SIZE) * Sheet::TILE_SIZE;
		tileBoundaries.width = Sheet::TILE_SIZE;
		tileBoundaries.height = Sheet::TILE_SIZE;

		m_sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo()
	{
		m_textureManager->releaseResource(m_tileSheetName);
	}

	sf::Sprite m_sprite;
	sf::Vector2i m_position;
	const unsigned int m_ID;
	const std::string m_tileSheetName;
	TextureManager* m_textureManager;
};

struct Tile
{
	Tile() 
		: m_deadly(false),
		m_warp(false)
	{}

	~Tile()
	{
		delete m_tileInfo;
	}

	TileInfo* m_tileInfo;
	bool m_deadly; //Harms entity
	bool m_warp; //Loads the next level
};

class Map
{
public:
	friend class StateGame;
	Map(const SharedContext& sharedContext);
	~Map();

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;

	const bool loadingNextMap() const { return m_nextLevel; }
	void loadNextLevel() { m_nextLevel = true; }
	const sf::Vector2u getMaxMapSize() const { return m_maxMapSize; }
	const sf::Vector2f getDefaultFriction() const { return m_defaultFriction; }
	const sf::Vector2f getDefaultGravity() const { return m_gravity; }
	const Tile* getTile(const unsigned int x, const unsigned int y) const;
	const int getTileSize() const { return m_tileSize; }

private:
	std::unordered_map<unsigned int, TileInfo*> m_tileSet;
	std::vector<std::pair<unsigned int, Tile*>> m_tileMap;
	std::unordered_map<std::string, std::string> m_maps;
	SharedContext m_sharedContext;
	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_defaultFriction;
	sf::Vector2f m_gravity;
	std::string m_backgroundName;
	sf::Sprite m_backgroundImage;
	bool m_nextLevel;
	void unloadCurrentMap();
	int m_currentMapIndex;
	std::array<std::string, static_cast<size_t>(MapNames::Total)> m_mapNames{ "Map1", "Map2" };
	int m_tileSize;

	const unsigned int convertCoords(const unsigned int x, const unsigned int y) const { return (x * m_maxMapSize.x) + y; }
	void loadMap(const std::string& mapName);
	void loadInTiles(const std::string& fileName);
	void loadInMaps(const std::string& fileName);
	void updateBackgroundPosition();
	void purgeTileSet();
	void purgeTileMap();

	void draw(sf::RenderWindow& window);
	void update();
};