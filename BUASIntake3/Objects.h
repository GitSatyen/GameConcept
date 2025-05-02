#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>
#include "Level.h"
class Objects
{
public:
	//Default Constructor
	Objects();
	void draw(sf::RenderTarget& image);
	void update(float deltaTime);
	void doAnime(float deltaTime);
	void setPosition(const sf::Vector2f& position);
	void setLevel(const Level& levelRef);

	sf::FloatRect getCollider() const;
	sf::Vector2i getGridPosition() const { return gridPosition; }

private:
	sf::Texture IdleAnim;
	sf::Sprite sprite; // Empty sprite object

	sf::Vector2i gridPosition;
	sf::Vector2i sourceImage;
	sf::Vector2f targetPosition;
	sf::Vector2f velocity;
	int tileSize = 32;
	float scale = 16.0f / 32.0f;
	float frameTime;
};

