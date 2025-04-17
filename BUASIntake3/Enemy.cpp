#include "Enemy.h"
#include "Level.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 

std::map <Enemy::State, std::string> stateMap = {
	{Enemy::State::None, "None"},
	{Enemy::State::Idle, "Idle"},
	{Enemy::State::Hurt, "Hurt"},
	{Enemy::State::Dead, "Dead"}
};

Enemy::Enemy()
{
	sourceImage = sf::Vector2i(0, 0);
	frameTime = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("Assets/Enemy/Idle.png")) {
		printf("Failed to load sprite texture\n");
	}

	else {
		printf("Sprite Found\n");
		IdleAnim.setSmooth(false);
		//Place texture in sprite object
		sprite.setTexture(IdleAnim);
		sprite.setTextureRect(sf::IntRect(0, 0, 68, 64));
		printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
	}
	
	// Center the sprite origin
	sprite.setOrigin( //Adjusting both values to fit inside Entity grid
		sprite.getLocalBounds().width / 5.5f, 
		sprite.getLocalBounds().height / 1.1f 
	);

	//Resizing enemy and adjusting sprite orientation to face right
	sprite.setScale(-std::abs(scale), scale);

	//Set Idle as default State
	setState(State::Idle);
}

void Enemy::setState(State newState)
{
	if (newState != state)
	{
		switch (newState)
		{
		//Calls action when traniston away from state
		case State::Idle:
			break;
		case State::Hurt:
			break;
		case State::Dead:
			break;
		}
		state = newState;
	}
}

void Enemy::draw(sf::RenderTarget& image)
{
	image.draw(sprite);

#ifndef NDEBUG
	// Draw collider 
	sf::FloatRect collider = getCollider();
	sf::RectangleShape rect(sf::Vector2f(collider.width, collider.height));
	rect.setPosition(collider.left, collider.top);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(2.0f);
	image.draw(rect);
#endif
}

void Enemy::update(float deltaTime)
{
	const int frameWidth = 68;
	const int totalFrames = 4;

	//Animate sprite frame by frame
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x = (sourceImage.x + 1) % totalFrames;
		sprite.setTextureRect(sf::IntRect(
		sourceImage.x * frameWidth,	0, frameWidth,64));
#ifndef NDEBUG
		//printf("Current Frame (Enemy): %d\n", sourceImage.x);
#endif
		frameTime = 0.0f;
	}
}

void Enemy::doAnime(float deltaTime)
{
	/*switch (state)
	{
	case State::Idle:
		IdleAnim.
		break;
	case State::Running:
		doRunning(deltaTime);
		break;
	case State::Jumping:
		doJumping(deltaTime);
		break;
	case State::Falling:
		doFalling(deltaTime);
		break;
	case State::Hit:
		doHit(deltaTime);
		break;
	case State::Dead:
		doDead(deltaTime);
		break;
	}*/
}

void Enemy::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f centeredPos(
		position.x + tileSize / 2.0f,
		position.y + tileSize / 2.0f
	);
	sprite.setPosition(centeredPos);
}

void Enemy::setLevel(const Level& levelRef)
{
	tileSize = levelRef.getWalkingGroundCellSize();
}

sf::FloatRect Enemy::getCollider() const
{
	// Custom collider dimensions (adjust these to match your sprite)
	const float colliderWidth = 32.0f;
	const float colliderHeight = 32.0f;
	
	// Calculate position based on sprite's center
	sf::Vector2f position = sprite.getPosition();
	return sf::FloatRect(
		position.x - colliderWidth,  // X position
		position.y - colliderHeight, // Y position
		colliderWidth,                   // Width
		colliderHeight                   // Height
	);
}

