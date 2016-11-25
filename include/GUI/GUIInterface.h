#pragma once

#include "GUI\GuiEntry.h"
#include "GUI\GuiStyle.h"
#include "States\StateType.h"
#include <vector>

class FontManager;
class GUIInterface
{
public:
	GUIInterface(const StateType interfaceType);
	~GUIInterface();

	GuiEntry* getGui(const std::string& message) const;

	const StateType getType() const { return m_type; }
	const std::string& getName() const { return m_name; }
	void setVisibility(const bool visible) { m_isVisible = visible; }
	void constructNewGuiElement(FontManager& fontManager, const GuiStyle& guiStyle, const std::string& message, const sf::Vector2f& position);
	void changeMessage(const std::string& existingMessage, const std::string& newMessage);
	
	//Need better ID system for GUI entries
	void removeGui(const std::string& message); //Destroy GUI by message, Will change at later date. 
	
	void draw(sf::RenderWindow& window);

private:
	std::vector<GuiEntry*> m_entries;
	const std::string m_name;
	const StateType m_type;
	bool m_isVisible;

	void purgeEntries();
};