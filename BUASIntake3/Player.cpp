#include "Player.h"
#include <SFML/Graphics.hpp>


Player::Player()
{
	player = sf::RectangleShape(sf::Vector2f(50, 50)); // Width: 200, Height: 100 
	player.setFillColor(sf::Color::Green);
	player.setPosition(300, 250);
	velocity = sf::Vector2f(0.0f, 0.0f);
}

void Player::draw(sf::RenderTarget& image)
{
	image.draw(player);
}

void Player::update(float deltaTime)
{
	Movement(deltaTime);
}

void Player::Movement(float deltaTime)
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	player.move(velocity * deltaTime);
	/*If the horizontal movement (left/right) is pressed and then the vertical movement button (up/down)
	gets pressed the character should be moving diagonally and vice versa */

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		player.move(-0.1, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		player.move(0.1, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player.move(0, -0.1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		player.move(0, 0.1);
	}
}
