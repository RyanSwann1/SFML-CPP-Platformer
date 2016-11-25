#include "GUI\GUIInterface.h"
#include "Managers\FontManager.h"
#include <algorithm>
#include <iostream>


GUIInterface::GUIInterface(const StateType interfaceType)
	: m_isVisible(false),
	m_type(interfaceType)
{}

GUIInterface::~GUIInterface()
{
	purgeEntries();
}

GuiEntry* GUIInterface::getGui(const std::string & message) const
{
	auto iter = std::find_if(m_entries.cbegin(), m_entries.cend(), [message](GuiEntry* guiEntry) {return guiEntry->getMessage() == message; });
	if (iter != m_entries.cend())
	{
		return *iter;
	}

	else {
		std::cout << "Couldn't find message.\n";
	}
	return nullptr;
}

void GUIInterface::changeMessage(const std::string & existingMessage, const std::string & newMessage)
{
	auto iter = std::find_if(m_entries.cbegin(), m_entries.cend(), [existingMessage](GuiEntry* guiEntry) {return guiEntry->getMessage() == existingMessage; });
	if (iter != m_entries.cend())
	{
		(*iter)->setNewMessage(newMessage);
	}
	else {
		std::cout << "Couldn't find existing message.\n";
	}
}

void GUIInterface::removeGui(const std::string & message)
{
	auto iter = std::find_if(m_entries.begin(), m_entries.end(), [message](GuiEntry* entry) { return entry->getMessage() == message; });
	if (iter != m_entries.cend())
	{
		delete *iter;
		m_entries.erase(iter);
	}
}

void GUIInterface::draw(sf::RenderWindow & window)
{
	if (m_isVisible)
	{
		for (const auto &i : m_entries)
		{
			i->draw(window);
		}
	}
}

void GUIInterface::constructNewGuiElement(FontManager& fontManager, const GuiStyle & guiStyle, const std::string & message, const sf::Vector2f & position)
{
	auto cIter = std::find_if(m_entries.cbegin(), m_entries.cend(), [message](GuiEntry* entry) {return entry->getMessage() == message; });
	if (cIter == m_entries.cend())
	{
		m_entries.emplace_back(new GuiEntry(fontManager, position, guiStyle, message));
	}
	else {
		std::cerr << "GUI Element already exists: " << message << "\n";
	}
}

void GUIInterface::purgeEntries()
{
	for (auto &i : m_entries)
	{
		delete i;
		i = nullptr;
	}
	m_entries.clear();
}
