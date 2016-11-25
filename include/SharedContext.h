#pragma once

class GUIManager;
class TextureManager;
class StateManager;
class EntityManager;
class EventManager;
class Window;
class AudioManager;
class Map;
class FontManager;

struct SharedContext
{
	SharedContext()
		: m_map(nullptr),
		m_textureManager(nullptr),
		m_stateManager(nullptr),
		m_entityManager(nullptr),
		m_eventManager(nullptr),
		m_window(nullptr),
		m_audioManager(nullptr),
		m_guiManager(nullptr),
		m_fontManager(nullptr) {}

	Map* m_map;
	TextureManager* m_textureManager;
	StateManager* m_stateManager;
	EntityManager* m_entityManager;
	EventManager* m_eventManager;
	Window* m_window;
	AudioManager* m_audioManager;
	GUIManager* m_guiManager;
	FontManager* m_fontManager;
};