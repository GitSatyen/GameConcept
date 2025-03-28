#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>

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

private:
	State state = State::None;

	sf::Texture IdleAnim;
	sf::Texture HurtAnim;
	sf::Texture DeadAnim;
	sf::Sprite sprite; // Empty sprite object


	sf::Vector2i sourceImage;
	sf::Clock clock;
	int hp = 1;
	sf::Vector2f velocity;
	float frameTime;
	float speed = 100.0f;
	//sf::RectangleShape player;
};

