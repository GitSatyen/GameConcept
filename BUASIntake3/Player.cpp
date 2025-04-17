#include "Player.h"
#include "Level.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp> 
#include <vector>


std::map <Player::State, std::string> stateMap = {
	{Player::State::None, "None"},
	{Player::State::Idle, "Idle"},
	{Player::State::Running, "Running"},
	{Player::State::Attack, "Attack"},
	{Player::State::Hurt, "Hurt"},
	{Player::State::Dead, "Dead"}
};

Player::Player() : tileSize(32)
{
	sourceImage = sf::Vector2i(0, 0);
	frameTime = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);

	//Define texture sprites for player sprite object 
	if (!IdleAnim.loadFromFile("Assets/Player/Idle.png")) {
		printf("Failed to load sprite texture\n");
	}
	if (!RunAnim.loadFromFile("Assets/Player/Walk.png")) {
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
						 sprite.getLocalBounds().height / 1.3f); //Adjusted to fit within the grid

		//Adjust scale to match grid size
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
			sprite.setTexture(IdleAnim);
			break;
		case State::Running:
			sprite.setTexture(RunAnim);
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
	switch (state)
	{
	case State::Idle:
		image.draw(sprite);
		break;
	case State::Running:
		image.draw(sprite);
		break;
	case State::Hurt:
		image.draw(sprite);
		break;
	case State::Dead:
		image.draw(sprite);
	}

#ifndef NDEBUG
	// Draw collider 
	sf::FloatRect collider = getCollider();
	sf::RectangleShape rect(sf::Vector2f(collider.width, collider.height));
	rect.setPosition(collider.left, collider.top);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(2.0f);
	image.draw(rect);

	// Draw anchor points
	std::vector<sf::Vector2f> anchors = getAnchorPoints();
	for (const auto& anchor : anchors) {
		sf::CircleShape point(3.0f); // Radius of 3 pixels
		point.setFillColor(sf::Color::Yellow);
		point.setPosition(anchor.x - 3.0f, anchor.y - 3.0f); // Center the circle
		image.draw(point);
	}
#endif // !NDEBUG

}

void Player::update(float deltaTime)
{
	Movement(deltaTime);

	//Player movement
	if (isMoving) { //Deepseek solution
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

	//Tranistion to idle when movement is finished
	if (!isMoving && state != State::Idle) {
		setState(State::Idle);
	}

	doAnime(deltaTime);
}

void Player::Movement(float deltaTime)
{
	//Prevent player from moving when he has won
	if (levelRef && levelRef->hasWon) {
		isMoving = false;
		return;
	}

	if (!keyProcessed) {
		sf::Vector2i newGridPosition = gridPosition;
		bool moved = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			newGridPosition.x-- * speed;
			moved = true;
			sprite.setScale(-std::abs(scale), scale); // Flip sprite left
			//std::cout << "Left pressed\n";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			newGridPosition.x++;
			moved = true;
			sprite.setScale(std::abs(scale), scale); // Flip sprite left
			//std::cout << "Right pressed\n";
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

		if (moved && level && level->isWalkingGround(newGridPosition.x, newGridPosition.y)) {
			targetPosition = sf::Vector2f(
				newGridPosition.x * tileSize + tileSize / 2.0f,
				newGridPosition.y * tileSize + tileSize / 2.0f
			);
			gridPosition = newGridPosition;
			isMoving = true;
			keyProcessed = true;
			setState(State::Running);
		}
	}
	//Deepseek solution
	else { //Reset the flag when all keys are released
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			keyProcessed = false;
		}
	}	

	Player::State currentState = getState();
#ifndef NDEBUG
	// Check state
	if (currentState == Player::State::Idle) {
		//printf("Idle");
	}
	else if (currentState == Player::State::Running) {
		//printf("Running");
	}
#endif 
}

void Player::doAnime(float deltaTime)
{
	// Switch animation based on state its in
	switch (state)
	{
	case State::Idle:
		doIdle(deltaTime);
		break;
	case State::Running:
		doRunning(deltaTime);
		break;
	case State::Hurt:
		doHurt(deltaTime);
		break;
	case State::Dead:
		doDead(deltaTime);
		break;
	}
}

void Player::doIdle(float deltaTime)
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
}

void Player::doRunning(float deltaTime)
{
	//Animate player sprite frame by frame
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x++;
		// Check sprite sheet width
		if (sourceImage.x * 128 >= RunAnim.getSize().x) {
			sourceImage.x = 0;
		}
		//Draw first frame of the spritesheet
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		frameTime = 0.0f;
	}

}

void Player::doAttack(float deltaTime)
{
}

void Player::doHurt(float deltaTime)
{
}

void Player::doDead(float deltaTime)
{
}

void Player::setStartPosition(const sf::Vector2f& position)
{
	gridPosition.x = static_cast<int>(position.x / tileSize);
	gridPosition.y = static_cast<int>(position.y / tileSize);
	targetPosition = sf::Vector2f(
		gridPosition.x * tileSize + tileSize / 2.0f,
		gridPosition.y * tileSize + tileSize / 2.0f
	);
	sprite.setPosition(position);
	std::cout << "Player initialized at grid: ("
		<< gridPosition.x << "," << gridPosition.y << ")\n";
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

void Player::setLevel(const Level& levelRef)
{
	//Deepseek solution
	this->levelRef = const_cast<Level*>(&levelRef);
	level = &levelRef;
	tileSize = level->getWalkingGroundCellSize();
}

sf::Vector2i Player::getGridPosition() const
{
	return gridPosition;
}

sf::FloatRect Player::getCollider() const
{
	//Deepseek solution
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

bool Player::checkCollsion(const sf::FloatRect& rect)
{
	return getCollider().intersects(rect);
}

std::vector<sf::Vector2f> Player::getAnchorPoints() const
{
	sf::FloatRect collider = getCollider();
	std::vector<sf::Vector2f> anchors;

	//Deepseek solution
	// Calculate anchor points at the four conrners
	anchors.emplace_back(collider.left, collider.top);                     // Top-left
	anchors.emplace_back(collider.left + collider.width, collider.top);    // Top-right
	anchors.emplace_back(collider.left, collider.top + collider.height);   // Bottom-left
	anchors.emplace_back(collider.left + collider.width, collider.top + collider.height); // Bottom-right
	
	return anchors;
}

bool Player::checkAnchorCollision(const sf::FloatRect& rect)
{
	const float padding = 5.0f;
	int collidingAnchors = 0;

	for (const auto& anchor : getAnchorPoints()) {
		// Create a small rectangle around each anchor point
		sf::FloatRect anchorRect(anchor.x - padding / 2,
			anchor.y - padding / 2,	padding,padding);
		
		if (rect.intersects(anchorRect)) {
			collidingAnchors++;
			if (collidingAnchors >= 2) return true;
		}
	}
	return false;
}
