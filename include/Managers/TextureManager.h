#pragma once

#include "ResourceManager.h"
#include "Utilities.h"
#include <SFML\Graphics.hpp>

class TextureManager
	: public ResourceManager<sf::Texture>
{
public:
	TextureManager()
	{
		loadInFilePath(Utilities::getPNGDirectory());
	}

	~TextureManager()
	{}

private:
	const sf::Texture* loadFromFile(const std::string& id) const override 
	{
		sf::Texture* texture = new sf::Texture;
		if (!texture->loadFromFile(id))
		{
			delete texture;
			texture = nullptr;
			return nullptr;
		}
		return texture;
	}

	void loadInFilePath(const std::string& fileName)
	{
		registerFilePath("TileSheet", fileName + "TileSheet.PNG");
		registerFilePath("PlayerSheet", fileName + "PlayerSheet.PNG");
		registerFilePath("RatSheet", fileName + "RatSheet.PNG");
		registerFilePath("Background", fileName + "Background.PNG");
	}
};