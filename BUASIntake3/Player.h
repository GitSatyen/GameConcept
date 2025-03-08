#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>

class Player
{
public:

	enum class State
	{
		None,
		Idle,
		Running,
		Attack,
		Hurt,
		Dead
	};

	// Default constructor
	Player();

	void setState(State newState);

	State getState() const noexcept
	{
		return state;
	}

	void draw(sf::RenderTarget& image);
	void update(float deltaTime);
	void Movement(float deltaTime);
	void doAnime(float deltaTime);

private:
	State state = State::None;

	sf::Texture IdleAnim;
	sf::Texture RunAnim;
	sf::Texture JumpAnim;
	sf::Texture FallAnim;
	sf::Texture HurtAnim;
	sf::Texture DeadAnim;
	sf::Sprite sprite; // Empty sprite object
	

	sf::Vector2i sourceImage;
	sf::Clock clock;
	int hp = 4;
	sf::Vector2f velocity;
	float frameTime;
	float speed = 100.0f;
	//sf::RectangleShape player;
};

