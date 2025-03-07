#include "Player.h"
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

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("Assets/Player/Idle.png")) {
		printf("Failed to load sprite texture\n");
	}

	else {
		printf("Sprite Found");
		//Place texture in sprite object
		sprite.setTexture(IdleAnim);

		//Animate sprite by frame
		sourceImage.x++;
		if (sourceImage.x * 64 >= IdleAnim.getSize().x){
			sourceImage.x = 0;
		};
		//Draw first frame of the spritesheet
		//sprite.setTextureRect(sf::IntRect(sourceImage.x * 64, sourceImage.y * 128, 128, 128));
		//Set sprite position
		sprite.setPosition(100, 250);
	}

	
	velocity = sf::Vector2f(0.0f, 0.0f);
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
	//player.move(velocity * deltaTime);
	Movement(deltaTime);

}

void Player::Movement(float deltaTime)
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	
	/*If the horizontal movement (left/right) is pressed and then the vertical movement button (up/down)
	gets pressed the character should be moving diagonally and vice versa */

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		velocity.x = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		velocity.x = speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		velocity.y = -speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		velocity.y = speed;
	}

	if (velocity.x != 0.0f && velocity.y != 0.0f) {
		velocity *= 0.7071f; // Normalize to maintain consistent speed diagonally
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
