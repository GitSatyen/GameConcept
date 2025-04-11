#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>
#include "Level.h"

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
	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doAttack(float deltaTime);
	void doHurt(float deltaTime);
	void doDead(float deltaTime);

	void setStartPosition(const sf::Vector2f& position);
	sf::Vector2f getPosition() const;
	void setGridPosition(int x, int y);
	void setLevel(const Level& levelRef);
	sf::Vector2i getGridPosition() const;

private:
	//Pointer Level class
	const Level* level = nullptr;

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

	bool keyProcessed = false;
	bool isMoving = false;

	int tileSize;
	int hp = 4;
	const float scale = 16.0f / 32.0f;
	float frameTime;
	float speed = 100.0f;
};