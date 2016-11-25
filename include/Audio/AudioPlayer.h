#pragma once

#include "SharedContext.h"
#include "Managers\AudioManager.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <SFML\Audio.hpp>

struct AudioClip
{
	AudioClip(AudioManager& audioManager, const std::string& ownerName, const std::string& audioClipName, sf::Sound& audioClip)
		: m_name(audioClipName),
		m_owner(ownerName),
		m_audioClip(&audioClip),
		m_audioManager(&audioManager)
	{
		m_audioClip->setBuffer(*m_audioManager->getResource(m_owner + m_name));
	}
	
	~AudioClip()
	{
		m_audioManager->releaseResource(m_name);
		delete m_audioClip;
		m_audioClip = nullptr;
	}

	const std::string m_owner;
	const std::string m_name;
	sf::Sound* m_audioClip;
	AudioManager* m_audioManager;
};

using AudioClips = std::vector<AudioClip*>;
struct SharedContext;
class AudioPlayer
{
public:
	AudioPlayer(const SharedContext& sharedContext);
	~AudioPlayer();

	void registerOwner(const std::string& ownerID);
	void registerAudioClip(const std::string& audioOwner, const std::string& ClipName);
	void removeAudioClip(const std::string& audioOwner, const std::string& audioClipName);
	void removeOwner(const std::string& audioOwner);
	void play(const std::string& audioClipName, const bool& loop);
	void stop(const std::string& audioClipName);

private:
	std::unordered_map<std::string, AudioClips> m_owners; //String is owner - IE'Player'
	SharedContext m_sharedContext;
	std::string m_currentOwner;
	AudioClips* m_currentAudioClips;

	sf::Sound* findAudioClip(const std::string& audioClipName) const;
	void loadInAudioClips(const std::string& fileName);
};