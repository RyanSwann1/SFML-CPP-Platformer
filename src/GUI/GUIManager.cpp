#include "Managers\GUIManager.h"
#include <iostream>


GUIManager::GUIManager()
	: m_currentInterface(nullptr)
{}

GUIManager::~GUIManager()
{
	purgeInterfaces();
	m_currentInterface = nullptr;
}

void GUIManager::addInterface(const StateType interfaceType , GUIInterface& guiInterface)
{
	auto iter = findInterface(interfaceType);
	if (!iter)
	{
		m_guiInterfaces.emplace(std::make_pair(interfaceType, &guiInterface));
	}
	else {
		std::cerr << "Interface already exists.\n";
	}
}

void GUIManager::removeInterface(const StateType interfaceType)
{
	auto iter = m_guiInterfaces.find(interfaceType);
	if (iter != m_guiInterfaces.cend())
	{
		if (m_currentInterface->getType() == interfaceType)
		{
			m_currentInterface = nullptr;
		}
		m_guiInterfaces.erase(iter);
	}
}

void GUIManager::setCurrentInterface(const StateType interfaceType)
{
	auto iter = findInterface(interfaceType);
	if (iter)
	{
		m_currentInterface = iter;
	}
	else
	{
		std::cerr << "Interface not found.\n";
	}
}

void GUIManager::setInterfaceVisibility(const StateType interfaceType, const bool visible)
{
	auto iter = findInterface(interfaceType);
	if (iter)
	{
		iter->setVisibility(visible);
	}
}

void GUIManager::draw(sf::RenderWindow & window)
{
	if (m_currentInterface)
	{
		m_currentInterface->draw(window);
	}
}

const bool GUIManager::interactingWithGuiEntry(const sf::Vector2f& mousePos, const std::string& message) const 
{
	if (m_currentInterface)
	{
		const GuiEntry* const guiEntry = m_currentInterface->getGui(message);
		if (guiEntry)
		{
			const sf::FloatRect guiEntryRect = guiEntry->getPosition();
			if (guiEntryRect.contains(mousePos))
			{
				return true;
			}
		}
	}
	
	return false;
}

GUIInterface * GUIManager::findInterface(const StateType interfaceType)
{
	auto cIter = m_guiInterfaces.find(interfaceType);
	return (cIter != m_guiInterfaces.cend() ? cIter->second : nullptr);
}

void GUIManager::purgeInterfaces()
{
	for (auto &i : m_guiInterfaces)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_guiInterfaces.clear();
}
