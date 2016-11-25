#pragma once

#include "Managers\FontManager.h"
#include "GuiStyle.h"
#include <SFML\Graphics.hpp>
#include <string>

struct GuiEntry
{
public:
	GuiEntry(FontManager& fontManager, const sf::Vector2f& position, const GuiStyle& guiStyle, const std::string& message);
	~GuiEntry();

	GuiEntry(const GuiEntry&);
	GuiEntry& operator=(const GuiEntry&);

	const std::string& getMessage() const { return m_message; }
	const std::string& getType() const { return m_type; }
	const sf::FloatRect getPosition() const;

	void setNewMessage(const std::string& newMessage) { m_message = newMessage; }
	void setPosition(const sf::Vector2f& newPos);

	void draw(sf::RenderWindow& window);

private:
	sf::RectangleShape m_shape;
	std::string m_message;
	std::string m_fontName;
	std::string m_type;
	sf::Text m_text;
	sf::Vector2f m_position;
	FontManager* m_fontManager;

	void init(const GuiEntry&);
};