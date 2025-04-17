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

   /* for (auto& enemy : enemies) {
            level.setEnemy(&enemy);
            enemy.update(deltaTime);
            enemy.draw(window);
        }*/

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
        level.setEnemy(&enemies.back()); // <-- Add this line
    }

    //Debugging player start spawn point
    sf::Vector2f startPos = level.getPlayerStartPosition();
    std::cout << "Start position: " << startPos.x << ", " << startPos.y << std::endl;
    std::cout << "Player position: " << player.getPosition().x << ", " << player.getPosition().y << std::endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
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
        window.setView(level.getGameView());
        window.clear(sf::Color::Black);
        level.draw(window);

        player.update(deltaTime);
        player.draw(window);

        princess.update(deltaTime);
        princess.draw(window);

        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
            enemy.draw(window);
        }

        

        level.updateCollision(deltaTime);

        // Reset view for UI elements if needed
        window.setView(window.getDefaultView());

        if (level.hasWon) {
            level.WonGame(window);
        }
        window.display();
    }
    return 0;
}

 

