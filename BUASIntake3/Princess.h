#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>
#include "Level.h"

class Princess
{
public:
	enum class State
	{
		None,
		Idle,
	};
	//Default constuctor
	Princess();
	void setState(State newState);

	State getState() const noexcept
	{
		return state;
	}

	void draw(sf::RenderTarget& image);
	void update(float deltaTime);
	void doAnime(float deltaTime);
	void setPosition(const sf::Vector2f& position);
	void setLevel(const Level& levelRef);

private:
	State state = State::None;

	sf::Texture IdleAnim;
	sf::Sprite sprite; // Empty sprite object

	sf::Vector2i gridPosition;
	sf::Vector2i sourceImage;
	sf::Vector2f targetPosition;
	sf::Vector2f velocity;
	int tileSize = 32;
	float scale;
	float frameTime;
	float speed = 100.0f;
};

