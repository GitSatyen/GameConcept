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
	void setStartPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;
	void setGridPosition(int x, int y);
	void moveToGridPosition(int x, int y);
	sf::Vector2i getGridPosition() const;

private:
	State state = State::None;

	sf::Texture IdleAnim;
	sf::Texture RunAnim;
	sf::Texture JumpAnim;
	sf::Texture FallAnim;
	sf::Texture HurtAnim;
	sf::Texture DeadAnim;
	sf::Sprite sprite; // Empty sprite object (player object refrence)
	

	sf::Vector2i sourceImage;
	sf::Clock clock;
	sf::Vector2i gridPosition;
	sf::Vector2f targetPosition;
	sf::Vector2f velocity;
	bool isMoving = false;
	const int tileSize = 16;
	int hp = 4;
	float frameTime;
	float speed = 100.0f;
	//sf::RectangleShape player;
};