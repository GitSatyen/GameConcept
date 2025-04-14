#include "Princess.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 

std::map <Princess::State, std::string> stateMap = {
	{Princess::State::None, "None"},
	{Princess::State::Idle, "Idle"}
};

Princess::Princess()
{
	sourceImage = sf::Vector2i(0, 0);
	frameTime = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("Assets/Princess/Idle.png")) {
		printf("Failed to load sprite texture\n");
	}

	else {
		printf("Sprite Found\n");
		IdleAnim.setSmooth(false);
		IdleAnim.setRepeated(false);
		//Place texture in sprite object
		sprite.setTexture(IdleAnim);
		//Set first frame of the animation
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
	}

	// Center the sprite origin
	sprite.setOrigin( //Adjusting both values to fit inside Entity grid
		sprite.getLocalBounds().width / 2.0f,
		sprite.getLocalBounds().height / 1.4f);

	//Resizing enemy and adjusting sprite orientation to face right
	sprite.setScale(-std::abs(scale), scale);

	//Set Idle as default State
	setState(State::Idle);
}

void Princess::setState(State newState)
{
	if (newState != state)
	{
		switch (newState)
		{
			//Calls action when traniston away from state
		case State::Idle:
			break;
		}
		state = newState;
	}
}

void Princess::draw(sf::RenderTarget& image)
{
	image.draw(sprite);
}

void Princess::update(float deltaTime)
{
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
#ifndef NDEBUG
	const int frameWidth = 160;
	const int totalFrames = 4;
	const int frameHeight = 128; //Deepseek solution

	printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
	printf("Frame: %d, Texture Rect: (%d, %d, %d, %d)\n",
		sourceImage.x,
		sourceImage.x * frameWidth,
		0,
		frameWidth,
		frameHeight);
#endif
}

void Princess::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f centeredPos(std::round(position.x), (position.y));
	sprite.setPosition(centeredPos);
}

void Princess::setLevel(const Level& levelRef)
{
	tileSize = levelRef.getWalkingGroundCellSize();
}

sf::FloatRect Princess::getCollider() const
{
	// Custom collider dimensions (adjust these to match your sprite)
	const float colliderWidth = 32.0f;
	const float colliderHeight = 32.0f;

	// Calculate position based on sprite's center and origin offset
	sf::Vector2f position = sprite.getPosition();
	return sf::FloatRect(
		position.x - colliderWidth / 2,        // X position
		position.y - colliderHeight / 2,       // Y position (matches origin offset)
		colliderWidth,                         // Width
		colliderHeight                         // Height
	);
}
