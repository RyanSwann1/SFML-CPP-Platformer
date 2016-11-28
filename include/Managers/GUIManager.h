#pragma once

#include "States\StateType.h"
#include <unordered_map>
#include <SFML\Graphics.hpp>


class GUIInterface;
class GUIManager
{
public:
	GUIManager();
	~GUIManager();

	GUIManager(const GUIManager&) = delete;
	GUIManager& operator=(const GUIManager&) = delete;

	void addInterface(const StateType interfaceType, GUIInterface& guiInterface);
	void removeInterface(const StateType interfaceType);
	void setCurrentInterface(const StateType interfaceType);
	void setInterfaceVisibility(const StateType interfaceType, const bool visible);

	const bool interactingWithGuiEntry(const sf::Vector2f& mousePos, const std::string& message) const;
	void draw(sf::RenderWindow& window);

private:
	std::unordered_map<StateType, GUIInterface*> m_guiInterfaces;
	GUIInterface* m_currentInterface;
	GUIInterface* findInterface(const StateType interfaceType);

	void purgeInterfaces();
};