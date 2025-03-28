#include "Enemy.h"
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
	}
	
	//Set sprite position
	sprite.setPosition(200, 250);
	sprite.setScale(1.2f, 1.2f);

	IdleAnim.setSmooth(false);
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
		printf("Current Frame (Enemy): %d\n", sourceImage.x);
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
