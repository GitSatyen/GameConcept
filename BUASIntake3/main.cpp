#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Save the princess");
    Player player;
    Level level("C:/Users/anoep/source/repos/BUASIntake3/BUASIntake3/Debug/Assets/BG/Levels.ldtk");
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::Black);
        level.draw(window);
        player.update(deltaTime);
        player.draw(window);
        window.display();   
    }

    return 0;
}
