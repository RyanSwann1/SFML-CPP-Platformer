#pragma once

#include <SFML\Window.hpp>

enum class EventType
{
	KeyPressed = sf::Event::KeyPressed,
	KeyReleased = sf::Event::KeyReleased,
	MouseButtonPressed = sf::Event::MouseButtonPressed,
	MouseButtonReleased = sf::Event::MouseButtonReleased
};