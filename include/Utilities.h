#pragma once
#include <string>
#include <random>

namespace Utilities
{
	inline int randomNumberGenerator(const int min, const int max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);

		return dis(gen);
	}

	inline const std::string getWorkingDirectory() 
	{
		return std::string("Resources");
	}

	inline const std::string getAudioDirectory() { return "Audio/"; }

	inline const std::string getPNGDirectory() { return "PNG/"; }

	inline const std::string getAssetDirectory() { return "Assets/"; }

	inline const std::string getMapDirectory() { return "Maps/"; }

	inline const std::string getFontDirectory() { return "Fonts/";  }
}