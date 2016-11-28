#pragma once

//if any of these header files change
//Or if any files they depend on change
//They must be recompiled
#include "SharedContext.h"
#include <string>
#include <SFML\Graphics.hpp>
#include <unordered_map>

//https://github.com/mozilla/BrowserQuest/tree/master/client

//LOAD IN TILE MAP XML TUTORIAL
//http://www.dreamincode.net/forums/topic/232396-c-tile-engine-from-scratch-part-4/
class Character;
class Animation;
enum class Direction;
enum class AnimationName;
class SpriteSheet
{
	friend class Character;
public:
	SpriteSheet(const SharedContext& sharedContext);
	~SpriteSheet();

	SpriteSheet(const SpriteSheet&) = delete;
	SpriteSheet& operator=(const SpriteSheet&) = delete;

	const int getTileSize() const { return m_tileSize; }
	const std::string& getName() const { return m_spriteSheetName; }
	void setAnimationType(const AnimationName animationName, const Direction dir);
	void setSpritePosition(const sf::Vector2f& pos) { m_sprite.setPosition(pos); }
	Animation* getCurrentAnimation() const { return m_currentAnimation; }
	
	void update(Character& character, const float deltaTime);
	void draw(sf::RenderWindow& window);

private:
	std::unordered_multimap<AnimationName, std::pair<Direction, Animation*>> m_animations;
	Animation* m_currentAnimation;
	SharedContext m_sharedContext;
	sf::IntRect m_drawTargetPos;
	sf::Sprite m_sprite;
	std::string m_spriteSheetName;
	int m_tileSize;
	sf::Vector2f m_scale;
	sf::Vector2u m_size;

	void loadIn(const std::string& fileName);
	void handleAnimations(Character& character);
	void cropSprite();
};