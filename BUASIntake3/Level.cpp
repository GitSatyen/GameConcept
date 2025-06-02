#include "Level.h"
#include "Player.h"
#include "Princess.h"
#include "Enemy.h"
#include "Objects.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <set>


ldtk::Project Level::loadProject(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open()) {
		throw std::runtime_error("Cannot open file: " + filepath);
	}
	// Read the file content
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	std::cout << "File content:\n" << content << "\n";

	// Load the ldtk Project
	ldtk::Project proj;
	try {
		std::cout << "Attempting to load project...\n";
		proj.loadFromFile(filepath);
		std::cout << "Project loaded successfully.\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load LDtk file: " << e.what() << "\n";
		throw;
	}

	return proj;
}

sf::Vector2f Level::getPlayerStartPosition() const
{
	try {
		const auto& entities = level.getLayer("Objects");
		const auto& startEntities = entities.getEntitiesByName("Start");

		if (!startEntities.empty()) {
			const auto& startPos = startEntities[0].get();
			return sf::Vector2f(startPos.getPosition().x + startPos.getSize().x / 2.0f,
								startPos.getPosition().y + startPos.getSize().y / 2.0f);
		}
		return sf::Vector2f(baseWidth / 2, baseHeight / 2);
	}
	catch (const std::exception& e) {
		std::cerr << "Couldnt find startEntity" << e.what() << "\n";
		throw;
	}

	return sf::Vector2f(baseWidth / 2, baseHeight / 2);
}

sf::Vector2f Level::getPrincessPosition() const
{
	try {
		const auto& entities = level.getLayer("Objects");
		const auto& startEntities = entities.getEntitiesByName("Princess");

		if (!startEntities.empty()) {
			const auto& startPos = startEntities[0].get();
			return sf::Vector2f(startPos.getPosition().x + startPos.getSize().x / 2.0f,
				startPos.getPosition().y + startPos.getSize().y / 2.0f);
		}
		return sf::Vector2f(baseWidth / 2, baseHeight / 2);
	}
	catch (const std::exception& e) {
		std::cerr << "Couldnt find startEntity" << e.what() << "\n";
		throw;
	}

	return sf::Vector2f(baseWidth / 2, baseHeight / 2);
}


Level::Level(const std::string& filepath, sf::RenderWindow& window) :
	project(loadProject(filepath)),
	world(project.allWorlds().empty() ? throw std::runtime_error("No worlds found in LDtk project") : project.allWorlds()[0]),
	level(world.allLevels().empty() ? throw std::runtime_error("No levels found in world") : world.allLevels()[0]),
	hasWon(false),
	hasLost(false),
	enemiesCleared(false)
{
	//Load Font
	if (!font.loadFromFile("BUASIntake3/Assets/Fonts/trajan-pro/TrajanPro-Bold.otf")) {
		std::cout << "ERROR: Couldn't load victory font\n";
	}

	//Get level dimensions
	baseWidth = level.size.x;
	baseHeight = level.size.y;

	//Setup game view to center of the window
	gameView.setSize(baseWidth, baseHeight);
	gameView.setCenter(baseWidth / 2, baseHeight / 2);

	std::cout << "World name: " << world.getName() << "\n";
	std::cout << "Level name: " << level.name << ", ID: " << level.iid << "\n";

	//Load tileset
	if (!tileset_texture.loadFromFile("BUASIntake3/Assets/BG/background.png")) {
		printf("Failed to load tileset texture");
	}
	tile_sprite.setTexture(tileset_texture);

	Resize(window);

	for (const auto& layer : level.allLayers()) {
		// Check if the layer is an entity layer
		if (layer.getType() == ldtk::LayerType::Entities) {
			const auto& entityLayer = layer.allEntities();
			for (const auto& entity : entityLayer) {
				// Access entity properties
				std::cout << "Entity ID: " << entity.iid << std::endl;
				std::cout << "Entity name: " << entity.getName() << std::endl;
				std::cout << "Position: (" << entity.getPosition().x << ", " << entity.getPosition().y << ")" << std::endl;
				std::cout << "Size: (" << entity.getSize().x << ", " << entity.getSize().y << ")" << std::endl;

				for (const auto& field : entity.allFields()) {
					//printf("Field ", field.name, "= ", field.type);
					std::cout << "Field " << field.name, "= ", field.type;
				}

				//Get position of all enemies
				if (entity.getName() == "Enemy") {
					sf::Vector2f pos(
						entity.getPosition().x + entity.getSize().x / 2.0f,
						entity.getPosition().y + entity.getSize().y / 2.0f
					);
					enemyPositions.push_back(pos);
				}
				//Get position of all objects
				if (entity.getName() == "Obstacle") {
					sf::Vector2f pos(
						entity.getPosition().x + entity.getSize().x / 2.0f,
						entity.getPosition().y + entity.getSize().y / 2.0f
					);
					objectPositions.push_back(pos);
				}
			}
		}
	}
}

void Level::draw(sf::RenderTarget& image)
{
	// Set the custom view before drawing
	image.setView(gameView);

	int tile_size = 16;
	//Parse background layer
	const auto& bg_layer = level.getLayer("BG");

	//Keep track of grid positions where we've already drawn a dot
	std::set<std::pair<int, int>> drawn_positions;  //drawn_positions is a set of unique(x, y) integer coordinate pairs.
													//Each pair represents a position(possibly on a 2D grid or map).
													//No two identical positions(e.g., { 2, 3 }) can exist in the set.
													//The set keeps all the positions sorted, using the default lexicographical order of std::pair.
	
	//Draw all tiles
	for (const auto& tile : bg_layer.allTiles()) {
		// Get the grid position of the tile
		int grid_x = tile.getGridPosition().x;
		int grid_y = tile.getGridPosition().y;

		int tileset_x = (tile.tileId % (tileset_texture.getSize().x / tile_size)) * tile_size;
		int tileset_y = (tile.tileId / (tileset_texture.getSize().x / tile_size)) * tile_size;
		tile_sprite.setTextureRect(sf::IntRect(tileset_x, tileset_y, tile_size, tile_size));
		tile_sprite.setPosition(tile.getGridPosition().x * tile_size, tile.getGridPosition().y * tile_size);
		image.draw(tile_sprite);
	}

	//Displays centerpoint of intGrid layer WalkingGround
	try {
		const auto& walkingLayer = level.getLayer("WalkingGround");
		const int grid_size = walkingLayer.getCellSize();
		//Deepseek solution
		for (int y = 0; y < walkingLayer.getGridSize().y; y++) {
			for (int x = 0; x < walkingLayer.getGridSize().x; x++) {
				const auto& value = walkingLayer.getIntGridVal(x, y).value;

				if (value != -1) {
					float center_X = x * grid_size + grid_size / 2.0f;
					float center_Y = y * grid_size + grid_size / 2.0f;
#ifndef NDEBUG
					sf::CircleShape centerDot(2.0f);
					centerDot.setFillColor(sf::Color::Green);
					centerDot.setPosition(center_X - 2.0f, center_Y - 2.0f);
					image.draw(centerDot);
#endif
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error drawing WalkingGround layer: " << e.what() << "\n";
	}

	//Acces enitity properties
	// Iterate through layers in the level
	for (const auto& layer : level.allLayers()) {
		// Check if the layer is an entity layer
		if (layer.getType() == ldtk::LayerType::Entities) {
			const auto& entityLayer = layer.allEntities();

			for (const auto& entity : entityLayer) {
				sf::Vector2f position = sf::Vector2f(
					static_cast<float>(entity.getPosition().x),
					static_cast<float>(entity.getPosition().y)
				);

				sf::Vector2f size = sf::Vector2f(
					static_cast<float>(entity.getSize().x),
					static_cast<float>(entity.getSize().y)
				);
				// Create a rectangle shape for the entity's bounding box
				sf::RectangleShape rect(size);
				rect.setPosition(position);
#ifndef NDEBUG  //ONLY WHILE DEBUGGING 
				// Set the rectangle's outline to red
				rect.setFillColor(sf::Color::Transparent); // No fill
				rect.setOutlineColor(sf::Color::Red);      // Red outline
				rect.setOutlineThickness(2.0f);            // 2px border thickness
				image.draw(rect);						   // Draw the rectangle	
				// Draw entity center point
				sf::CircleShape centerDot(2.0f);
				centerDot.setFillColor(sf::Color::Cyan);
				centerDot.setPosition(
					position.x + size.x / 2.0f - 2.0f,
					position.y + size.y / 2.0f - 2.0f
				);
				image.draw(centerDot);

#endif			//ONLY WHILE DEBUGGING 
			}
		}	
	}
}

void Level::Resize(sf::RenderWindow& window)
{
	//Get all window and level dimensions
	sf::Vector2u windowSize = window.getSize();
	//Calculate aspect ratios
	float windowAspect = static_cast<float>(windowSize.x) / windowSize.y;
	float levelAspect = baseWidth / baseHeight;

	if (windowAspect > levelAspect) {
		//Make Window fit to height
		gameView.setSize(baseWidth * windowAspect, baseHeight);
	}
	else
	{
		//Make Window fit to width
		gameView.setSize(baseWidth, baseWidth / windowAspect);
	}
	//Center Window
	gameView.setCenter(baseWidth / 2, baseHeight / 2);
}

bool Level::isWalkingGround(int gridX, int gridY) const
{
	//Deepseek solution
	try {
		const auto& walkingLayer = level.getLayer("WalkingGround");
		// Check if grid coordinates are within bounds (Deepseek solution)
		if (gridX < 0 || gridY < 0 ||
			gridX >= walkingLayer.getGridSize().x ||
			gridY >= walkingLayer.getGridSize().y) {
			std::cout << "Out of bounds: (" << gridX << ", " << gridY << ")\n";
			return false;
		}
		// Check if the tile is walkable (not blocked in WalkingGround layer)
		bool isWalkable = (walkingLayer.getIntGridVal(gridX, gridY).value != -1);
		// Check if there's an object at this grid position
		bool hasObject = (getObjectAtGrid(gridX, gridY) != nullptr);
		std::cout << "Tile (" << gridX << "," << gridY << "): "
			<< (isWalkable ? "Walkable" : "Blocked") << "\n";

		// Return when tile is walkable (value != -1)
		return (walkingLayer.getIntGridVal(gridX, gridY).value != -1) && !hasObject;

	}
	catch (const std::exception& e) {
		std::cerr << "Error returning WalkingGround layer: " << e.what() << "\n";
		return false; // Invalid if layer/tile not found
	}
}

int Level::getWalkingGroundCellSize() const
{
	auto& gridSize = level.getLayer("WalkingGround");
	try {
		const auto& walkingLayer = level.getLayer("WalkingGround");
		return walkingLayer.getCellSize();
	}
	catch (const std::exception& e) {
		std::cerr << "Error getting WalkingGround cell size: " << e.what() << "\n";
		return NULL;
	}
}

sf::Vector2i Level::getGridSize() const
{
	auto& gridSize = level.getLayer("WalkingGround");
	try {
		const auto& walkingLayer = level.getLayer("WalkingGround");
		return sf::Vector2i(
			walkingLayer.getGridSize().x,
			walkingLayer.getGridSize().y
		);
	}
	catch (const std::exception& e) {
		std::cerr << "Error getting grid size: " << e.what() << "\n";
		return sf::Vector2i(0, 0);
	}
}

Enemy* Level::getEnemyAtGrid(int gridX, int gridY) const
{
	//Deepseek solution
	for (Enemy* enemy : enemies) {
		if (enemy && enemy->getGridPosition().x == gridX && enemy->getGridPosition().y == gridY) {
			return enemy;
		}
	}
	return nullptr;
}

Objects* Level::getObjectAtGrid(int gridX, int gridY) const
{
	for (Objects* object : objects) {
		if (object && object->getGridPosition().x == gridX && object->getGridPosition().y == gridY) {
			return object;
		}
	}
	return nullptr;
}

void Level::setPlayer(Player* playerRef)
{
	player = playerRef;
}

void Level::setPrincess(Princess* princessRef)
{
	princess = princessRef;
}

void Level::setEnemy(Enemy* EnemyRef)
{
	enemies.push_back(EnemyRef);
}

void Level::setObject(Objects* ObjectRef)
{
	objects.push_back(ObjectRef);
}


void Level::updateCollision(float deltaTime)
{
	//Reset collision states
	colldingWithEnemy = false; 
	leftEdgeIsColliding = false;
	rightEdgeIsColliding = false;
	topEdgeIsColliding = false;
	bottomEdgeIsColliding = false;

	//When colliding with Princess
	if (player && princess) {
		bool coll_With_Princess = player->checkCollsion(princess->getCollider());
		//Player wins when all enemies are cleared
		if (coll_With_Princess && enemiesCleared) {
			hasWon = true;
			std::cout << "Princess saved!\n";
		}
		//Dies if they aren't all cleared
		else if (coll_With_Princess && !enemiesCleared) {
			hasLost = true;
			player->isDead = true;
			player->setState(Player::State::Dead);
		}
	}

	//When colliding with an Enemy
	if (player) {
		for (Enemy* enemy : enemies) {
			if (enemy && enemy->getState() != Enemy::State::Dead) {
				bool coll_With_Enemy = player->checkAnchorCollision(enemy->getCollider());
				if (coll_With_Enemy) {
					//Get collider edges from the Player
					sf::FloatRect playerLeftEdge = player->getLeftEdge();
					sf::FloatRect playerRightEdge = player->getRightEdge();
					sf::FloatRect playerTopEdge = player->getTopEdge();
					sf::FloatRect playerBottomEdge = player->getBottomEdge();

					//Get enemy collider from the Enemy
					sf::FloatRect enemyCollider = enemy->getCollider();
					// Check collision on each side
					if (playerLeftEdge.intersects(enemyCollider)) {
						//std::cout << "Left collider on enemy";
						colldingWithEnemy = true;
						leftEdgeIsColliding = true;

						if (colldingWithEnemy && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
							player->setState(Player::State::Attack);
							enemy->setState(Enemy::State::Dead);
							player->turns--;

							if (!player->getkeyProcessed()) {
								player->setkeyProcessed(true); // Prevent multiple decrements
							}
						}
					}
					else if (playerRightEdge.intersects(enemyCollider)) {
						//std::cout << "Right collider on enemy";
						colldingWithEnemy = true;
						rightEdgeIsColliding = true;

						if (colldingWithEnemy && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
							player->setState(Player::State::Attack);
							enemy->setState(Enemy::State::Dead);
							player->turns--;

							if (!player->getkeyProcessed()) {
								player->setkeyProcessed(true); // Prevent multiple decrements
							}
						}
					}
					else if (playerTopEdge.intersects(enemyCollider)) {
						//std::cout << "Upper collider on enemy";
						colldingWithEnemy = true;
						topEdgeIsColliding = true;

						if (colldingWithEnemy && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
							player->setState(Player::State::Attack);
							enemy->setState(Enemy::State::Dead);
							player->turns--;

							if (!player->getkeyProcessed()) {
								player->setkeyProcessed(true); // Prevent multiple decrements
							}
						}
					}
					else if (playerBottomEdge.intersects(enemyCollider)) {
						//std::cout << "Lower collider on enemy";
						colldingWithEnemy = true;
						bottomEdgeIsColliding = true;

						if (colldingWithEnemy && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
							player->setState(Player::State::Attack);
							enemy->setState(Enemy::State::Dead);
							player->turns--;

							if (!player->getkeyProcessed()) {
								player->setkeyProcessed(true); // Prevent multiple decrements
							}
						}
					}
				}
			}
		}
	}

	//Reset keyProcessed if not colliding
	if (!colldingWithEnemy && player) {
		player->setkeyProcessed(false);
	}

	//Check if player has no more turns
	if (player && player->turns <= 0) {
		player->isDead = true;
		player->setState(Player::State::Dead);
		hasLost = true;
	}

	//Check if death animation been has been completed
	if (player && player->isDeathAnimationComplete()) {
		hasLost = true;
	}

	//Deepseek solution
	//Remove dead enemies
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[](Enemy* enemy) { return enemy->getState() == Enemy::State::Dead;}),
		enemies.end());

	enemiesCleared = enemies.empty();
#ifndef NDEBUG
	if (enemiesCleared) { std::cout << "Enemies Cleared\n"; }
#endif
}

void Level::playerTurnCountDown(sf::RenderTarget& window, int countdown)
{
	//std::cout << player->turns;
	text.setFont(font);
	text.setString("Turns: " + std::to_string(countdown));
	text.setCharacterSize(12);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOrigin(0, 0);

	// Center the text in the view
	sf::Vector2f viewTopLeft = window.getView().getCenter() - window.getView().getSize() / 2.0f;
	text.setPosition(viewTopLeft.x + 10, viewTopLeft.y + 10);
	window.draw(text);
}

void Level::resetGameState()
{
	hasWon = false;
	hasLost = false;

	// Reset player
	if (player) {
		player->deadAnimeFinished = false;
		player->isDead = false;
		player->resetAnimationFrame();
		player->resetFrameTimer();
		player->setStartPosition(getPlayerStartPosition());
		player->turns = 17;
#ifndef NDEBUG
		player->turns = 50;
#endif // !NDEBUG

		player->setState(Player::State::Idle);
	}
}

void Level::gameWon(sf::RenderTarget& window)
{
	if (!font.loadFromFile("BUASIntake3/Assets/Fonts/trajan-pro/TrajanPro-Bold.otf"))
	{
		std::cerr << "Font not found\n";
		return;
	}

	sf::Text options;
	std::cout << "You win!\n";
	options.setFont(font);
	text.setFont(font);
	
	options.setString("Quit [Q]");
	text.setString("You win!");
	
	options.setCharacterSize(20);
	text.setCharacterSize(48);
	
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);

	// Center the text in the view
	sf::Vector2f winCenter = window.getView().getCenter();
	text.setPosition(winCenter.x - 100, winCenter.y);
	options.setPosition(winCenter.x - 50, winCenter.y + 100);
#ifndef NDEGUG
	std::cout << "Text position: " << text.getPosition().x
		<< ", " << text.getPosition().y << "\n";
#endif // !NDEGUG

	// Draw semi-transparent background
	sf::RectangleShape background(window.getView().getSize());
	background.setFillColor(sf::Color(0, 0, 0, 200));  // Semi-transparent black
	window.draw(background);

	// Draw the text
	window.draw(text);
	window.draw(options);
}

void Level::gameOver(sf::RenderWindow& window)
{
	text.setFont(font);
	text.setString("Game Over!\nPress R to Restart\nQ to Quit");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);

	// Center text
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.width / 2, textBounds.height / 2);
	text.setPosition(window.getView().getCenter());

	// Draw transparent background
	sf::RectangleShape background(window.getView().getSize());
	background.setFillColor(sf::Color(0, 0, 0, 200));
	window.draw(background);

	window.draw(text);
}

void Level::gamePaused(sf::RenderWindow& window)
{
	text.setFont(font);
	text.setString("Continue [ESC]\nRestart [R]\nInstructions [F]\nQuit [Q]");
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);

	// Center text
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.width / 2, textBounds.height / 2);
	text.setPosition(window.getView().getCenter());

	// Draw transparent background
	sf::RectangleShape background(window.getView().getSize());
	background.setFillColor(sf::Color(0, 0, 0, 200));
	window.draw(background);

	window.draw(text);
}

void Level::gameInstructions(sf::RenderWindow& window)
{
	text.setFont(font);
	text.setString(R"(
					 Use the arrow keys to move 
                     Don't turn out of turns!
					 Defeat all enemies to please the princess!
					 Restart [R]
					 Quit [Q]
					 
					 Close [F])");
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);

	// Center text
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin((textBounds.width / 2) + 50, textBounds.height / 2);
	text.setPosition(window.getView().getCenter());

	// Draw transparent background
	sf::RectangleShape background(window.getView().getSize());
	background.setFillColor(sf::Color(0, 0, 0, 200));
	window.draw(background);

	window.draw(text);
}
