#pragma once

#include "Managers\ResourceManager.h"
#include "Utilities.h"
#include <SFML\Audio.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

class AudioManager 
	: public ResourceManager<sf::SoundBuffer>
{
public:
	AudioManager()
	{
		loadIn(Utilities::getAudioDirectory());
	}

	~AudioManager() 
	{
		purgeResources();
	}

private:
	const sf::SoundBuffer* loadFromFile(const std::string& fileName) const override
	{
		sf::SoundBuffer* soundBuffer = new sf::SoundBuffer;
		if (!soundBuffer->loadFromFile(fileName))
		{
			delete soundBuffer;
			soundBuffer = nullptr;
			return nullptr;
		}
		return soundBuffer;
	}

	void loadIn(const std::string& fileName)
	{
		registerFilePath("PlayerJump", fileName + "PlayerJump.Wav");
		registerFilePath("PlayerAttack", fileName + "PlayerAttack.Wav");
		registerFilePath("PlayerDeath", fileName + "PlayerDeath.Wav");
		registerFilePath("RatDeath", fileName + "RatDeath.Wav");
		registerFilePath("PlayerHurt", fileName + "PlayerHurt.Wav");
		registerFilePath("GameBackgroundMusic", fileName + "GameBackgroundMusic.FLAC");
	}
};