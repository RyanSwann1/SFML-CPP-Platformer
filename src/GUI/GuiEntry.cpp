#include "GUI\GuiEntry.h"
#include <iostream>

GuiEntry::GuiEntry(FontManager& fontManager, const sf::Vector2f& position, const GuiStyle& guiStyle, const std::string& message)
	: m_fontManager(&fontManager),
	m_message(message),
	m_fontName(guiStyle.m_fontName),
	m_position(position)
{
	m_shape.setFillColor(guiStyle.m_bodyColor);
	m_shape.setPosition(position);
	m_shape.setSize(guiStyle.m_size);

	m_text.setFont(*m_fontManager->getResource(m_fontName));
	m_text.setString(m_message);
	m_text.setCharacterSize(guiStyle.m_characterSize);
	m_text.setColor(sf::Color::Black);
	m_text.setPosition(position);
	m_text.setOrigin(guiStyle.m_size.x / 2.0f, guiStyle.m_size.y / 2.0f);
}

GuiEntry::~GuiEntry()
{
	m_fontManager->releaseResource(m_fontName);
}

//Assign unitilized varaibles
GuiEntry::GuiEntry(const GuiEntry & orig)
	: m_shape(orig.m_shape),
	m_text(m_text), 
	m_fontName(orig.m_fontName),
	m_position(orig.m_position),
	m_fontManager(orig.m_fontManager),
	m_type(orig.m_type)
{
	m_text.setFont(*orig.m_fontManager->getResource(orig.m_fontName));
}

//Assign already initialized variables
GuiEntry & GuiEntry::operator=(const GuiEntry & entry)
{
	m_shape = entry.m_shape;
	m_text = entry.m_text;
	m_text.setFont(*m_fontManager->getResource(entry.m_fontName));
	m_message = entry.m_message;
	m_fontName = entry.m_fontName;
	m_position = entry.m_position;
	m_fontManager = entry.m_fontManager;
	m_type = entry.m_type;
	return *this;
}

void GuiEntry::draw(sf::RenderWindow & window)
{
	window.draw(m_shape);
	window.draw(m_text);
}

const sf::FloatRect GuiEntry::getPosition() const
{
	const sf::Vector2f size = m_shape.getSize();
	const sf::Vector2f pos = m_shape.getPosition();
	const sf::FloatRect rect(pos.x, pos.y, size.x, size.y);
	return rect;
}

void GuiEntry::setPosition(const sf::Vector2f & newPos)
{
	m_shape.setPosition(newPos);
	m_text.setPosition(newPos);

	//Possibly change the origin too
	//Set both at same position, box is bigger should work out correctly
}