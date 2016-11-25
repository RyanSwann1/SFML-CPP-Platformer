#pragma once

#include "Managers\EventManager.h"
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <string>

//https://www.reddit.com/r/programming/comments/4nrsw1/is_dark_theme_in_ides_actually_worse_for_you/

struct SharedContext;
class Window
{
public:
	Window(SharedContext& sharedContext, const sf::Vector2u& size = sf::Vector2u(900, 600), const std::string& name = "SFML_WINDOW");
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void beginDraw() { m_window.clear(sf::Color::Black); }
	void draw(sf::Drawable& drawable) { m_window.draw(drawable); }
	void endDraw() { m_window.display(); }
	const bool isRunning() const { return m_isRunning; }
	void stopRunning() { m_isRunning = false; }
	void close() { m_window.close(); }

	void setup(const sf::Vector2u& size, const std::string& name);
	void update();

	sf::RenderWindow& getWindow() { return m_window; }
	EventManager& getEventManager() { return m_eventManager; }

	const sf::FloatRect& getViewSpace() const;

private:
	sf::RenderWindow m_window;
	sf::Vector2u m_size;
	std::string m_name;
	EventManager m_eventManager;
	bool m_fullScreen;
	bool m_isRunning;

	void create();
};

