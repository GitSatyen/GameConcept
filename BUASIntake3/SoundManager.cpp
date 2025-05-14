#include "SoundManager.h"
#include <SFML/Audio.hpp>
#include <iostream> 

SoundManager::SoundManager()
{

}

void SoundManager::loadSound()
{
    // Load each sound file into its buffer
    soundBuffers[SoundType::MOVE].loadFromFile("BUASIntake3/Assets/Sounds/movement.mp3");
    soundBuffers[SoundType::ATTACK].loadFromFile("BUASIntake3/Assets/Sounds/Attack.mp3");
    soundBuffers[SoundType::PLAYER_HURT].loadFromFile("BUASIntake3/Assets/Sounds/player_hurt.mp3");
    soundBuffers[SoundType::GAME_OVER].loadFromFile("BUASIntake3/Assets/Sounds/player_dies.mp3");
    soundBuffers[SoundType::GAME_WON].loadFromFile("BUASIntake3/Assets/Sounds/wongame.mp3");

    if (!soundBuffers[SoundType::MOVE].loadFromFile("BUASIntake3/Assets/Sounds/movement.mp3")) {
        std::cout << "Couldn't load sound";
    }

    for (auto& pair : sounds) {
        pair.second.setVolume(10);
    }

    // Assign buffers to sounds
    for (auto& pair : soundBuffers) {
        sounds[pair.first].setBuffer(pair.second);
    }
}

void SoundManager::playSound(SoundType type)
{
    if (sounds.count(type) > 0) {
        sounds[type].play();  // Play the sound once
    }
}
