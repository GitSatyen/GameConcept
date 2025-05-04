#include "Player.h"
#include "Level.h"
#include "Enemy.h"
#include "GameStates.h"
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
	if (!AttackAnim.loadFromFile("Assets/Player/Attack_3.png")) {
		printf("Failed to load sprite texture\n");
	}
	if (!DeadAnim.loadFromFile("Assets/Player/Dead.png")) {
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
		// Grok fix
		// Reset animation frame data when transitioning to Idle
		if (newState == State::Idle || newState == State::Attack) {
			sourceImage.x = 0; // Reset frame index
			frameTime = 0.0f;  // Reset frame timer
			sprite.setTextureRect(sf::IntRect(sourceImage.x * 128,
				sourceImage.y * 128, 128, 128));
		}

		switch (newState)
		{
		//Calls action when traniston away from state
		case State::Idle:
			sprite.setTexture(IdleAnim);
			break;
		case State::Running:
			sprite.setTexture(RunAnim);
			sounds.playSound(SoundManager::SoundType::MOVE);
			break;
		case State::Attack:
			sprite.setTexture(AttackAnim);
			isMoving = false; //Prevent movement during attack 
			break;
		case State::Hurt:
			break;
		case State::Dead:
			sprite.setTexture(DeadAnim);
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
		printf("Idle\n");
		break;
	case State::Running:
		image.draw(sprite);
		printf("Running\n");
		break;
	case State::Attack:
		image.draw(sprite);
		printf("Attack\n");
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
	// Reset requireKeyRelease when all keys are released
	if (requireKeyRelease) {
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			requireKeyRelease = false;
		}
		else { return; }
	}

	//Make Player attack
	if (state == State::Attack) {
		doAttack(deltaTime);

		//Check if attack animation completed
		if (sourceImage.x >= AttackAnim.getSize().x / 128 - 1) {
			isAttacking = false;
			//Prevent and reset movement to allow new movement
			setState(State::Idle);
			keyProcessed = false;
			requireKeyRelease = true;
		}
	}

	else if(isMoving){
		//Move towards the target position
		sf::Vector2f direction = targetPosition - sprite.getPosition();
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (distance < 5.0f) { // Close enough to snap to position
			sprite.setPosition(targetPosition);
			isMoving = false;
			gridPosition = sf::Vector2i(
				static_cast<int>(targetPosition.x / tileSize),
				static_cast<int>(targetPosition.y / tileSize)
			);
			setState(State::Idle);
			requireKeyRelease = true;
		}
		else {
			direction /= distance; //Grok fix
			sprite.move(direction * speed * deltaTime);
		}
	}
	else {
		if (state == State::Idle && state != State::Attack && !requireKeyRelease) {
			Movement(deltaTime);
		}
	}

	//Make Player die 
	if (turns < 1) {
		isDead = true;
	}

	if (isDead) {
		setState(State::Dead);
		doDead(deltaTime);
	}

	//Tranistion to idle when movement is finished
	if (!isMoving && state != State::Idle && !isDead && state != State::Attack) {
		setState(State::Idle);
	}

	doAnime(deltaTime);
}

void Player::Movement(float deltaTime)
{
	if (state == State::Attack) { return; }

	//Prevent player from moving when he has won
	if (levelRef && (levelRef->hasWon || levelRef->hasLost) || paused) {
		return;
	}

	//Wait for all keys to be released if required
	if (requireKeyRelease) {
		if (!isMoving && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			requireKeyRelease = false;
		}
		return;
	}

#ifndef NDEBUG 
	// Check edge collisions via Level's getters
	if (levelRef->isRightEdgeColliding()) {
		std::cout << "Player's right edge is colliding with an enemy!\n";
	}
#endif // !NDEBUG 
	
	if (!keyProcessed && state != State::Attack) {
		sf::Vector2i newGridPosition = gridPosition;
		bool moved = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !level->isLeftEdgeColliding())
		{
			if (state != State::Attack) {
				std::cout << "Left key pressed, moving to (" << newGridPosition.x - 1 << ", " << newGridPosition.y << ")\n";
				newGridPosition.x--;
				moved = true;
				sprite.setScale(-std::abs(scale), scale); // Flip sprite left
				//std::cout << "Left pressed\n";
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !level->isRightEdgeColliding())
		{
			if (state != State::Attack) {
				std::cout << "Right key pressed, moving to (" << newGridPosition.x + 1 << ", " << newGridPosition.y << ")\n";
				newGridPosition.x++;
				moved = true;
				sprite.setScale(std::abs(scale), scale); // Flip sprite left
				//std::cout << "Right pressed\n";
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !level->isTopEdgeColliding())
		{
			newGridPosition.y--;
			moved = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !level->isBottomEdgeColliding())
		{
			newGridPosition.y++;
			moved = true;
		}

		//Make sure player can't move outside the WalkingGround layer
		if (moved && level && level->isWalkingGround(newGridPosition.x, newGridPosition.y)) {
			//Check for enemy FIRST before attacking
			Enemy* targetEnemy = level->getEnemyAtGrid(newGridPosition.x, newGridPosition.y);
			gridPosition = newGridPosition;
			targetPosition = sf::Vector2f(
				newGridPosition.x * tileSize + tileSize / 2.0f,
				newGridPosition.y * tileSize + tileSize / 2.0f
			);
			isMoving = true;
			setState(State::Running);
			turns--;
			keyProcessed = true; //Decrease a turn on movement
		}
	}
	//Deepseek solution
	else { //Reset the flag when all keys are released
		if (!isMoving && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
			!sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			keyProcessed = false;
		}
	}	
}

void Player::Attack(float deltaTime)
{
	setState(State::Attack);

#ifndef NDEBUG
	Player::State currentState = getState();
	// Check state
	if (currentState == Player::State::Idle) {
		printf("Idle");
	}
	else if (currentState == Player::State::Running) {
		printf("Attacking");
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
	//Animate player sprite frame by frame
	frameTime += deltaTime;

	if (frameTime >= 0.2f) {
		if (sourceImage.x < AttackAnim.getSize().x / 128 - 1) { //Grok fix
			sourceImage.x++;
		}
		// Check sprite sheet width
		if (sourceImage.x * 128 >= AttackAnim.getSize().x) {
			sourceImage.x = 0;
		}
		//Draw first frame of the spritesheet
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		frameTime = 0.0f;
	}
}

void Player::doHurt(float deltaTime)
{
}

void Player::doDead(float deltaTime)
{
	//Animate player sprite frame by frame
	deadAnimeFinished = false;
	frameTime += deltaTime;
	if (frameTime >= 0.2f) {
		sourceImage.x++;
		// Check sprite sheet width
		if (sourceImage.x * 128 >= DeadAnim.getSize().x) {
			deadAnimeFinished = true;
			//Stay on last frame
			sourceImage.x--;
		}
		//Draw first frame of the spritesheet
		sprite.setTextureRect(sf::IntRect(sourceImage.x * 128, sourceImage.y * 128, 128, 128));
		frameTime = 0.0f;
	}
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

sf::FloatRect Player::getLeftEdge() const
{
	sf::FloatRect collider = getCollider();
	return sf::FloatRect(
		collider.left - 1.f,
		collider.top,
		1.f, // Thin width (1 pixel)
		collider.height
	);
}

sf::FloatRect Player::getRightEdge() const
{
	sf::FloatRect collider = getCollider();
	return sf::FloatRect(
		collider.left + collider.width,
		collider.top,
		1.f,
		collider.height
	);
}

sf::FloatRect Player::getTopEdge() const
{
	sf::FloatRect collider = getCollider();
	return sf::FloatRect(
		collider.left,
		collider.top - 1.f,
		collider.width,
		1.f // Thin height (1 pixel)
	);
}

sf::FloatRect Player::getBottomEdge() const
{
	sf::FloatRect collider = getCollider();
	return sf::FloatRect(
		collider.left,
		collider.top + collider.height,
		collider.width,
		1.f
	);
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

bool Player::isRightEdgeCollidingWith(const sf::FloatRect& otherCollider) const
{
	// Get the player's right edge
	sf::FloatRect rightEdge = getRightEdge();

	// Check intersection with the given collider
	return rightEdge.intersects(otherCollider);
}
