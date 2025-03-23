#include "Level.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include "Player.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

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

	// Load the ldtk::Project
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


Level::Level(const std::string& filepath) :
	project(loadProject(filepath)),
	world(project.allWorlds().empty() ? throw std::runtime_error("No worlds found in LDtk project") : project.allWorlds()[0]),
	level(world.allLevels().empty() ? throw std::runtime_error("No levels found in world") : world.allLevels()[0])
{

	std::cout << "World name: " << world.getName() << "\n";
	std::cout << "Level name: " << level.name << ", ID: " << level.iid << "\n";

	//Load tileset
	if (!tileset_texture.loadFromFile("Assets/BG/background.png")) {
		printf("Failed to load tileset texture");
	}
	tile_sprite.setTexture(tileset_texture);
}

void Level::draw(sf::RenderTarget& image)
{
	int tile_size = 16;
	//Parse background layer
	const auto& bg_layer = level.getLayer("BG");
	for (const auto& tile : bg_layer.allTiles()) {
		int tileset_x = (tile.tileId % (tileset_texture.getSize().x / tile_size)) * tile_size;
		int tileset_y = (tile.tileId / (tileset_texture.getSize().x / tile_size)) * tile_size;
		tile_sprite.setTextureRect(sf::IntRect(tileset_x, tileset_y, tile_size, tile_size));
		tile_sprite.setPosition(tile.getGridPosition().x * tile_size, tile.getGridPosition().y * tile_size);
		image.draw(tile_sprite);
	}

	//Acces enitity properties
	// Iterate through layers in the level
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
					printf("Field ", field.name, "= ", field.type);
				}

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
				rect.setOutlineThickness(2.0f);           // 2px border thickness
				image.draw(rect);						  // Draw the rectangle	
#endif			//ONLY WHILE DEBUGGING 
			}
		}
	}
}