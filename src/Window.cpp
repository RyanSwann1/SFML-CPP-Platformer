#include "Window.h"
#include "SharedContext.h"

Window::Window(SharedContext& sharedContext, const sf::Vector2u & size, const std::string & name)
	: m_eventManager(sharedContext),
	m_isRunning(true)
{
	setup(size, name);
}

Window::~Window()
{}

void Window::setup(const sf::Vector2u& size, const std::string& name)
{
	m_name = name;
	m_size = size;
	m_fullScreen = false;
	create();
}

void Window::update()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
			m_isRunning = false;
		}
		m_eventManager.handleEvent(event);
	}
	m_eventManager.update();
}

const sf::FloatRect& Window::getViewSpace() const
{
	const sf::Vector2f viewSpaceCenter = m_window.getView().getCenter();
	const sf::Vector2f viewSpaceSize = m_window.getView().getSize();
	const sf::Vector2f viewSizeHalf(viewSpaceSize.x / 2.0f, viewSpaceSize.y / 2.0f);
	return sf::FloatRect(viewSpaceCenter - viewSizeHalf, viewSpaceSize);
}

void Window::create()
{
	auto style = (m_fullScreen ? sf::Style::Fullscreen : sf::Style::Default);
	m_window.create(sf::VideoMode(m_size.x, m_size.y), m_name, style);
}