#include "Audio\AudioPlayer.h"
#include "Managers\AudioManager.h"
#include "SharedContext.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

AudioPlayer::AudioPlayer(const SharedContext& sharedContext)
	: m_sharedContext(sharedContext),
	m_currentAudioClips(nullptr)
{}

AudioPlayer::~AudioPlayer()
{}

void AudioPlayer::registerOwner(const std::string & ownerID)
{
	m_currentOwner = ownerID;
	auto cIter = m_owners.find(ownerID);
	if (cIter != m_owners.cend())
	{
		m_currentAudioClips = &cIter->second;
	}
	else 
	{
		std::cerr << "\nOwner not found.\n";
	}
}

void AudioPlayer::registerAudioClip(const std::string& audioOwner, const std::string & audioClipName)
{
	AudioManager& audioManager = *m_sharedContext.m_audioManager;
	sf::Sound* const audioClip = new sf::Sound;
	auto owner = m_owners.find(audioOwner);
	if (owner != m_owners.cend())
	{	
		owner->second.emplace_back(new AudioClip(audioManager, audioOwner, audioClipName, *audioClip));		
	}
	else
	{
		m_owners.emplace(audioOwner, std::vector<AudioClip*>{new AudioClip(audioManager, audioOwner, audioClipName, *audioClip)});
	}
}

void AudioPlayer::removeAudioClip(const std::string & audioOwner, const std::string & audioClipName)
{
	auto iter = m_owners.find(audioOwner);
	if (iter != m_owners.cend())
	{
		auto audioClip = std::find_if(iter->second.begin(), iter->second.end(), [audioClipName](AudioClip* audioClip) {return audioClip->m_name == audioClipName; });
		if (audioClip != iter->second.end())
		{
			iter->second.erase(audioClip);
			return;
		}

	}
}

void AudioPlayer::removeOwner(const std::string & audioOwner)
{
	AudioManager& audioManager = *m_sharedContext.m_audioManager;
	auto iter = m_owners.find(audioOwner);
	if (iter != m_owners.cend())
	{
		m_owners.erase(iter);
	}
}

void AudioPlayer::play(const std::string & audioClipName, const bool& loop)
{
	sf::Sound* const audioClip = findAudioClip(audioClipName);
	if (audioClip)
	{
		audioClip->play();
		audioClip->setLoop(loop);
	}
}

void AudioPlayer::stop(const std::string & audioClipName)
{
	sf::Sound* const audioClip = findAudioClip(audioClipName);
	if (audioClip)
	{
		audioClip->stop();
	}
}

sf::Sound* AudioPlayer::findAudioClip(const std::string & audioClipName) const
{
	if (m_currentAudioClips)
	{
		auto cIter = std::find_if(m_currentAudioClips->cbegin(), m_currentAudioClips->cend(), [audioClipName](AudioClip* audioClip) {return audioClip->m_name == audioClipName; });
		if (cIter != m_currentAudioClips->cend())
		{
			return (*cIter)->m_audioClip;
		}
	}

	auto owner = m_owners.find(m_currentOwner);
	if (owner != m_owners.cend())
	{
		auto audioClips = owner->second;
		auto cIter = std::find_if(audioClips.cbegin(), audioClips.cend(), [audioClipName](AudioClip* audioClip) {return audioClip->m_name == audioClipName; });
		if (cIter != audioClips.cend())
		{
			return (*cIter)->m_audioClip;
		}/*
		for (auto audioClip = audioClips.begin(); audioClip != audioClips.end(); ++audioClip)
		{
			if ((*audioClip)->m_name == audioClipName)
			{
				return (*audioClip)->m_audioClip;
			}
		}*/
	}

	//not found
	return nullptr;
}

void AudioPlayer::loadInAudioClips(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cerr << "Couldn't open file: " << fileName << "\n";
		return;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);

		std::string owner;
		std::string audioClipName;

	}
	file.close();
}