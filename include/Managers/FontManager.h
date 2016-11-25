#pragma once

#include "Managers\ResourceManager.h"
#include "Utilities.h"
#include <SFML\Graphics.hpp>

class FontManager 
	: public ResourceManager<sf::Font>
{
public:
	FontManager()
	{
		loadIn(Utilities::getFontDirectory());
	}

	~FontManager()
	{}

	const sf::Font* loadFromFile(const std::string& fileName) const override
	{
		sf::Font* font = new sf::Font;
		if (!font->loadFromFile(fileName))
		{
			delete font;
			font = nullptr;
			return nullptr;
		}
		return font;
	}
	 
private:
	void loadIn(const std::string& fileName)
	{
		registerFilePath("Magnificent", fileName + "Magnificent.ttf");
	}
};