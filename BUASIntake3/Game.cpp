#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream> 

Game::Game(Level& levelRef, Player& playerRef, Princess& princessRef,
    std::vector<Enemy>& enemiesRef, std::vector<Objects>& objectsRef)
    : level(levelRef), player(playerRef), princess(princessRef),
    enemies(enemiesRef), objects(objectsRef){
}

void Game::resetGame()
{
    //All Deepseek solution
    // Clear existing entities
    objects.clear();
    enemies.clear();
    level.clearEnemies();
    level.clearObjects();

    // Recreate enemies
    enemies.reserve(level.getEnemyPositions().size());
    for (const auto& pos : level.getEnemyPositions()) {
        enemies.emplace_back();
        enemies.back().setPosition(pos);
        enemies.back().setLevel(level);
        level.setEnemy(&enemies.back());
    }

    // Recreate objects
    objects.reserve(level.getObjectPositions().size());
    for (const auto& pos : level.getObjectPositions()) {
        objects.emplace_back();
        objects.back().setPosition(pos);
        objects.back().setLevel(level);
        level.setObject(&objects.back());
    }

    // Reset game state
    level.resetGameState();
    player.setStartPosition(level.getPlayerStartPosition());
    princess.setPosition(level.getPrincessPosition());
}
