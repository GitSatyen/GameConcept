#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	// Default constructor
	Player();

	void draw(sf::RenderTarget& image);
	void update(float deltaTime);
	void Movement(float deltaTime);

private:
	

	int hp = 4;
	sf::Vector2f velocity;
	float speed = 100.0f;
	sf::RectangleShape player;
};

