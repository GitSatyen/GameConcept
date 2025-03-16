#include "Level.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include "Player.h"
#include <string>
#include <fstream>

ldtk::Project Level::loadProject(const std::string& filepath)
{
	ldtk::Project project;
	//Load LDtk project file
	try {
		project.loadFromFile(filepath);
		// Print all world names
		const auto& worlds = project.allWorlds();
		std::cout << "Number of worlds: " << worlds.size() << "\n";
		for (const auto& w : worlds) {
			std::cout << "World: " << w.getName() << "\n";
			for (const auto& l : w.allLevels()) {
				std::cout << "  Level: " << l.name << "\n";
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load LDtk file: " << e.what() << "\n";
	}
	return project;
}

Level::Level(const std::string& filepath) :project(loadProject(filepath)) {
	//world(project.allWorlds().empty() ? throw std::runtime_error("No worlds found in LDtk project") : project.allWorlds()[0]),
	//level(world.allLevels().empty() ? throw std::runtime_error("No levels found in world") : world.allLevels()[0])
	

	auto worlds = project.allWorlds();
	if (worlds.empty()) {
		throw std::runtime_error("No world found in LDtk project");
	}
	world = worlds[0];
	printf("World accessed:", world.getName());

	std::cout << "Accessing levels...\n";
	auto levels = world.allLevels();
	if (levels.empty()) {
		throw std::runtime_error("No levels found in world");
	}
	level = levels[0];
	printf("Level accessed", level.getName());

	//Load LDtk project file
	try {
		project.loadFromFile(filepath);
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load LDtk file: " << e.what() << "\n";
		return;
	}

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
}
