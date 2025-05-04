#pragma once
#include <SFML/Audio.hpp>
#include "Level.h"
#include "Player.h"
#include "Princess.h"
#include "Enemy.h"
#include "Objects.h"

class Game
{
public:
	Game(Level& levelRef, Player& playerRef, Princess& princessRef,
		std::vector<Enemy>& enemiesRef, std::vector<Objects>& objectsRef);
	void resetGame();

private:
	Level& level;
	Player& player;
	Princess& princess;
	std::vector<Enemy>& enemies;
	std::vector<Objects>& objects;
};

