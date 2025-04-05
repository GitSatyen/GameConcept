#include <fstream>
#include <iostream>  // For std::cerr and std::cout
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "LDtkLoader/Project.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Save the princess");
    // Set minimum size (will enforce through event handling)
    sf::Vector2u minWindowSize(400, 300);

    Player player;
    Enemy enemy;
    Level level("Assets/BG/Levels.ldtk", window);
    sf::Clock clock;
    bool isFullscreen = false;

    //Link Level to Player
    player.setLevel(level);
    //Set player position of the start entity
    player.setStartPosition(level.getStartPosition());

    //Debugging player start spawn point
    sf::Vector2f startPos = level.getStartPosition();
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
        enemy.update(deltaTime);
        enemy.draw(window);

        // Reset view for UI elements if needed
        window.setView(window.getDefaultView());
        window.display();
    }
    return 0;
}

 

