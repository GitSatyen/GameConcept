#include "Princess.h"

std::map <Princess::State, std::string> stateMap = {
	{Princess::State::None, "None"},
	{Princess::State::Idle, "Idle"}
};

Princess::Princess() : scale(1.0f)
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
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 160, sourceImage.y * 128, 160, 128));
		printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
	}

	// Center the sprite origin
	sprite.setOrigin( //Adjusting both values to fit inside Entity grid
		sprite.getLocalBounds().width / 2.0f,
		sprite.getLocalBounds().height / 2.0f);

	//Resizing enemy and adjusting sprite orientation to face right
	//sprite.setScale(-std::abs(scale), scale);
	sprite.setScale(scale, scale);
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
	//image.draw(sprite);
#ifndef NDEBUG
	// Draw frame boundary box
	sf::RectangleShape frame(sf::Vector2f(
		sprite.getGlobalBounds().width,
		sprite.getGlobalBounds().height
	));
	frame.setPosition(sprite.getPosition());
	frame.setOrigin(sprite.getOrigin());
	frame.setFillColor(sf::Color::Transparent);
	frame.setOutlineColor(sf::Color::Yellow);
	frame.setOutlineThickness(1.0f);
	image.draw(frame);
#endif

	image.draw(sprite);
}

void Princess::update(float deltaTime)
{
	const int frameWidth = 160;
	const int totalFrames = 4;
	const int frameHeight = 128; //Deepseek solution

	//Animate sprite frame by frame
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x = (sourceImage.x + 1) % totalFrames;
		// Validate texture coordinates
		const int maxX = (IdleAnim.getSize().x - frameWidth) / frameWidth;
		if (sourceImage.x > maxX) sourceImage.x = 0;

		sprite.setTextureRect(sf::IntRect(
			sourceImage.x * frameWidth, 0, frameWidth, frameHeight));
#ifndef NDEBUG
		printf("Texture Size: %dx%d\n", IdleAnim.getSize().x, IdleAnim.getSize().y);
		printf("Frame: %d, Texture Rect: (%d, %d, %d, %d)\n",
			sourceImage.x,
			sourceImage.x * frameWidth,
			0,
			frameWidth,
			frameHeight);
#endif
		frameTime = 0.0f;
	}
}

void Princess::setPosition(const sf::Vector2f& position)
{
	sf::Vector2f centeredPos(std::round(position.x), (position.y));
	sprite.setPosition(centeredPos);
}

void Princess::setLevel(const Level& levelRef)
{
	tileSize = levelRef.getWalkingGroundCellSize();
	// Calculate scale based on tile size and original frame width (160px)
	const float widthScale = static_cast<float>(tileSize) / 160.0f;  // Frame width
	const float heightScale = static_cast<float>(tileSize) / 128.0f; // Frame height
	scale = std::min(widthScale, heightScale);  // Maintain aspect ratio

	// Apply new scale
	sprite.setScale(scale, scale);

	// Recenter origin after scaling
	sprite.setOrigin(
		(sprite.getLocalBounds().width) / 2.0f * scale,
		(sprite.getLocalBounds().height) / 2.0f * scale
	);
}
