#pragma once

#include <SFML\Graphics.hpp>
#include <string>

//Acts as one big data structure
struct GuiStyle
{
	GuiStyle() {}
	GuiStyle(const sf::Color& bodyColor, const int characterSize, const sf::Vector2f& size, const std::string& fontName = "")
		: m_bodyColor(bodyColor),
		m_fontName(fontName),
		m_characterSize(characterSize),
		m_size(size)
	{}

	sf::Color m_bodyColor;
	int m_characterSize;
	std::string m_fontName;
	sf::Vector2f m_size;
};