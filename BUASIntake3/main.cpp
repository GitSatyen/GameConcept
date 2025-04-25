#include <fstream>
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "Princess.h"
#include "LDtkLoader/Project.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Save the princess");
    // Set minimum size (will enforce through event handling)
    sf::Vector2u minWindowSize(400, 300);

    Player player;
    Princess princess;
    std::vector<Enemy> enemies;
    Level level("Assets/BG/Levels.ldtk", window);
    sf::Clock clock;
    bool isFullscreen = false;

    // Pass references to the Level (containing collission logic)
    level.setPlayer(&player);
    level.setPrincess(&princess);

    // Pass Level to the Player (containing movement logic)
    player.setLevel(level);
    //Set player position of the start entity
    player.setStartPosition(level.getPlayerStartPosition());

    //Do same with princess
    princess.setLevel(level);
    princess.setPosition(level.getPrincessPosition());

    //Deepseek fix
    // Reserve space to avoid reallocation
    enemies.reserve(level.getEnemyPositions().size());
    //Set positions of enemies
    for (const auto& pos : level.getEnemyPositions()) {
        enemies.emplace_back();
        enemies.back().setPosition(pos);
        enemies.back().setLevel(level);
        level.setEnemy(&enemies.back());
    }

    //Debugging player start spawn point
    sf::Vector2f startPos = level.getPlayerStartPosition();
    std::cout << "Start position: " << startPos.x << ", " << startPos.y << std::endl;
    std::cout << "Player position: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;

    //Deepseek solution
    // Initial game setup function
    auto setupGame = [&]() {
        level.resetGameState();
        player.setStartPosition(level.getPlayerStartPosition());
        princess.setPosition(level.getPrincessPosition());
        enemies.clear(); //Clear existing enemies
        level.clearEnemies();

        //Recreate enemies from their positions
        enemies.reserve(level.getEnemyPositions().size());
        for (const auto& pos : level.getEnemyPositions()) {
            enemies.emplace_back();
            enemies.back().setPosition(pos);
            enemies.back().setLevel(level);
            level.setEnemy(&enemies.back());
        }
    };
    setupGame();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    setupGame();
                }
                //Handle game state transitions
                if (level.hasWon || level.hasLost) {
                    
                    if (event.key.code == sf::Keyboard::Q) {
                        window.close();
                    }
                }
                //Handle fulscreen mode
                if (event.key.code == sf::Keyboard::F11) {
                    isFullscreen = !isFullscreen;
                    window.close();
                    window.create(isFullscreen ?
                        sf::VideoMode::getDesktopMode() : sf::VideoMode(800, 600),
                        "Save the princess",
                        (isFullscreen ? sf::Style::Fullscreen : sf::Style::Default));
                    window.setVerticalSyncEnabled(true);
                    level.Resize(window);
                    window.setView(level.getGameView());
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        // Only update game logic if not in completed state
        if (!level.hasWon && !level.hasLost) {
            // Update level collision
            level.updateCollision(deltaTime);
        }

        window.setView(level.getGameView());
        window.clear(sf::Color::Black);
        //Draw map and objects
        level.draw(window);
        player.update(deltaTime);
        player.draw(window);
        princess.update(deltaTime);
        princess.draw(window);
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
            enemy.draw(window);
        }

        //Initilize game functions
        // Always draw UI elements
        level.playerTurnCountDown(window, player.turns);

        if (level.hasWon) {
            level.gameWon(window);
        }
        else if (level.hasLost) {
            level.gameOver(window);
            player.draw(window);
        }
        window.display();
    }
    return 0;
}

 

