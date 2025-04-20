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
	void Attack(float deltaTime);

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

	sf::FloatRect getCollider() const;
	bool checkCollsion(const sf::FloatRect& rect);

	std::vector<sf::Vector2f> getAnchorPoints() const; 
	bool checkAnchorCollision(const sf::FloatRect& rect);
	bool isDeathAnimationComplete() const { return deadAnimeFinished; }

	//Public methods to refrence private variables
	void resetAnimationFrame() { sourceImage.x = 0; }
	void resetFrameTimer() { frameTime = 0.0f; }

	//Other getters and setters 
	bool getkeyProcessed() const { return keyProcessed; }
	void setkeyProcessed(bool val) { keyProcessed = val; }
	
	uint32_t turns; //Experimental unsigned int. Value should not be negative
	bool deadAnimeFinished = false;
	bool isDead = false;

private:
	//Pointer Level class
	const Level* level = nullptr;
	Level* levelRef = nullptr; //Modiviable value

	State state = State::None;

	sf::Texture IdleAnim;
	sf::Texture RunAnim;
	sf::Texture AttackAnim;
	sf::Texture FallAnim;
	sf::Texture HurtAnim;
	sf::Texture DeadAnim;
	sf::Sprite sprite; // Empty sprite object (player object refrence)

	sf::Vector2i sourceImage;
	sf::Clock clock;
	sf::Vector2i gridPosition;
	sf::Vector2f targetPosition;
	sf::Vector2f velocity;

	//Movement values
	bool keyProcessed = false;
	bool isMoving = false;
	bool canMove = true;

	//Attacking values
	bool isAttacking = false;
	bool attackCompleted = false;
	sf::Vector2i attackTargetGrid;

	int tileSize;
	const float scale = 16.0f / 32.0f;
	float frameTime;
	float speed = 100.0f;
	
};