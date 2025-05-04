#pragma once
#include <SFML/Audio.hpp>
#include <iostream> 
#include <unordered_map>

class SoundManager
{
public:
	//Grok solution
	enum class SoundType {
		MOVE,
		ATTACK,
		PLAYER_HURT,
		GAME_OVER,
		GAME_WON
	};

	SoundManager();
	void loadSound();
	void playSound(SoundType soundType);

private:
	//Deepseek solution
	std::unordered_map<SoundType, sf::SoundBuffer> soundBuffers;
	std::unordered_map<SoundType, sf::Sound> sounds;
};

