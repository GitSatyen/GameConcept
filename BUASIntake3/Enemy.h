#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>
#include "Level.h"

class Enemy
{
public:
	enum class State
	{
		None,
		Idle,
		Attack,
		Hurt,
		Dead
	};
	
	//Default constuctor 
	Enemy();
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

	sf::FloatRect getCollider() const; //Replica from Princess class

private:
	State state = State::None;

	sf::Texture IdleAnim;
	sf::Texture HurtAnim;
	sf::Texture DeadAnim;
	sf::Sprite sprite; // Empty sprite object

	sf::Vector2i gridPosition;
	sf::Vector2i sourceImage;
	sf::Vector2f targetPosition;
	sf::Vector2f velocity;
	sf::Clock clock;
	int tileSize = 32;
	int hp = 1;
	const float scale = 16.0f / 23.0f;
	float frameTime;
	float speed = 100.0f;
};

