#include "Level.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include "Player.h"
#include <string>
#include <fstream>

Level::Level(const std::string& filepath) : 
	project(), 
	world(project.getWorld("World")),
	level(world.getLevel("Level1"))
{
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
