#include "Objects.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <SFML/Graphics/Sprite.hpp>
#include "Level.h"

Objects::Objects()
{
	sourceImage = sf::Vector2i(0, 0);
	frameTime = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("BUASIntake3/Assets/Props/barrel.png")) {
		printf("Failed to load sprite texture\n");
	}
	else {
		printf("Sprite Found\n");
		sprite.setTexture(IdleAnim);
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 34, sourceImage.y * 34, 34, 34));
	}

	// Center the sprite origin
	sprite.setOrigin( //Adjusting both values to fit inside Entity grid
		sprite.getLocalBounds().width / 2.0f,
		sprite.getLocalBounds().height / 2.0f);
}

void Objects::draw(sf::RenderTarget& image)
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

void Objects::update(float deltaTime)
{
	//Animate sprite frame by frame
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x++;
		// Check sprite sheet width
		if (sourceImage.x * 34 >= IdleAnim.getSize().x) {
			sourceImage.x = 0;
		}
		//Draw first frame of the spritesheet
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 34, sourceImage.y * 34, 34, 34));
		frameTime = 0.0f;
	}
#ifndef NDEBUG
	const int frameWidth = 160;
	const int totalFrames = 4;
	const int frameHeight = 34; //Deepseek solution

	/*printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
	printf("Frame: %d, Texture Rect: (%d, %d, %d, %d)\n",
		sourceImage.x,
		sourceImage.x * frameWidth,
		0,
		frameWidth,
		frameHeight);*/
#endif
}

void Objects::setPosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);

	//Calculate grid position
	gridPosition.x = static_cast<int>(position.x / tileSize);
	gridPosition.y = static_cast<int>(position.y / tileSize);
}

void Objects::setLevel(const Level& levelRef)
{
	tileSize = levelRef.getWalkingGroundCellSize();
}

sf::FloatRect Objects::getCollider() const
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
