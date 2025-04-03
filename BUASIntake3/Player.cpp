#include "Player.h"
#include "Level.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 


std::map <Player::State, std::string> stateMap = {
	{Player::State::None, "None"},
	{Player::State::Idle, "Idle"},
	{Player::State::Running, "Running"},
	{Player::State::Attack, "Attack"},
	{Player::State::Hurt, "Hurt"},
	{Player::State::Dead, "Dead"}
};

Player::Player()
{
	sourceImage = sf::Vector2i(0, 0);
	frameTime = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("Assets/Player/Idle.png")) {
		printf("Failed to load sprite texture\n");
	}

	else {
		printf("Sprite Found\n");
		//Place texture in sprite object
		sprite.setTexture(IdleAnim);
	}
		//Set first frame of the animation
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		
		//Center sprite origin
		sprite.setOrigin(sprite.getLocalBounds().width / 2.0f,
						 sprite.getLocalBounds().height * 0.75f);

		//Adjust scale to match grid size
		float scale = 16.0f / 32.0f;
		sprite.setScale(scale, scale);

	//Set Idle as default State
	setState(State::Idle);
}

void Player::setState(State newState)
{
	if (newState != state)
	{
		switch (newState)
		{
		//Calls action when traniston away from state
		case State::Idle:
			break;
		case State::Running:
			break;
		case State::Hurt:
			break;
		case State::Dead:
			break;
		}
		state = newState;
	}
}

void Player::draw(sf::RenderTarget& image)
{
	image.draw(sprite);
	//switch (state)
	//{
	//case State::Idle:
	//	//image.draw(IdleAnim);
	//	break;
	//case State::Running:
	//	//image.draw(RunAnim);
	//	break;
	//case State::Hurt:
	//	//image.draw(HurtAnim);
	//	break;
	//case State::Dead:
	//	//image.draw(DeadAnim);
	//}
}

void Player::update(float deltaTime)
{
	Movement(deltaTime);

	//Player movement
	if (isMoving) {
		//Move towards the target position
		sf::Vector2f direction = targetPosition - sprite.getPosition();
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	
		if (distance < 5.0f) { // Close enough to snap to position
			sprite.setPosition(targetPosition);
			isMoving = false;
		}
		else {
			// Normalize the direction and move
			if (distance > 0) {
				direction.x /= distance;
				direction.y /= distance;
			}
			sprite.move(direction * speed * deltaTime);
		}
	}

	//Animate player sprite frame by frame
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x++;
		// Check sprite sheet width
		if (sourceImage.x * 128 >= IdleAnim.getSize().x) { 
			sourceImage.x = 0;
		}
		//Draw first frame of the spritesheet
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		frameTime = 0.0f;
	}
}

void Player::Movement(float deltaTime)
{
	if(isMoving) return; // Don't process new movement while already moving

	sf::Vector2i newGridPosition = gridPosition;
	bool moved = false;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		newGridPosition.x--;
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		newGridPosition.x++;
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		newGridPosition.y--;
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		newGridPosition.y++;
		moved = true;
	}

	if (moved) {
		moveToGridPosition(newGridPosition.x, newGridPosition.y); // Normalize to maintain consistent speed diagonally
	}
}

void Player::doAnime(float deltaTime)
{
	// Switch animation based on state its in
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

void Player::setStartPosition(const sf::Vector2f& position)
{
	sf::Vector2i gridPos(
		static_cast<int>(position.x / tileSize),
		static_cast<int>(position.y / tileSize)
	);
	setGridPosition(gridPos.x, gridPos.y);

	//Directly set pixel position from entity coordinates
	sprite.setPosition(position);
	targetPosition = position;

}

sf::Vector2f Player::getPosition() const
{
	return sprite.getPosition();
}

void Player::setGridPosition(int x, int y)
{
	gridPosition = sf::Vector2i(x, y);
	targetPosition = sf::Vector2f(
		x * tileSize + tileSize / 2.0f,	y * tileSize + tileSize / 2.0f);
	sprite.setPosition(targetPosition);
}

void Player::moveToGridPosition(int x, int y)
{
	if (!isMoving) {
		gridPosition.x = x;
		gridPosition.y = y;
		// Calculate the center position of the target tile
		targetPosition.x = x * tileSize + tileSize / 2.0f - sprite.getLocalBounds().width / 2.0f;
		targetPosition.y = y * tileSize + tileSize / 2.0f - sprite.getLocalBounds().height / 2.0f;
		isMoving = true;
	}
}

sf::Vector2i Player::getGridPosition() const
{
	return gridPosition;
}
