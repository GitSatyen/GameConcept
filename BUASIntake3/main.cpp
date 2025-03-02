#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collide");
    sf::RectangleShape rectangle(sf::Vector2f(200, 100)); // Width: 200, Height: 100 
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setPosition(300, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();   
    }

    return 0;
}
