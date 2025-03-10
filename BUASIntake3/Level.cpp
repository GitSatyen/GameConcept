#include "Level.h"
#include "LDtkLoader/Project.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Image.hpp"
#include "Player.h"
#include <string>
#include <fstream>

Level::Level()
{
	//ldtk::Project
}

void Level::loadFile(const std::string& filepath)
{
	//Load LDtk project file
	project.loadFromFile("Assets/BG/Levels.ldtk");

	//Get world and level from project
	const auto& world = project.getWorld("World");
	const auto& level = world.getLevel("Level1");

	//Parse all layers
	const auto& bg_layer = level.getLayer("BG"); //Background layer
	for (const auto& tile : bg_layer.allTiles()) {
		// Access tile properties: position, texture coordinates, etc.
		int x = tile.getGridPosition().x; // Grid-based X position
		int y = tile.getGridPosition().y; // Grid-based Y position
		int tile_id = tile.tileId; // ID in the tileset
	}
}

void Level::draw(sf::RenderTarget& image)
{
	tile_sprite.setTexture(tileset_texture);
	if (!tileset_texture.loadFromFile("Assets/BG/background.png")) {
		printf("No tileset found");
	}

	int tile_size = 16;
	for (const auto& tile : level.getLayer("BG").allTiles()) {
		int tileset_x = (tile.tileId % (tileset_texture.getSize().x / tile_size)) * tile_size;
		int tileset_y = (tile.tileId / (tileset_texture.getSize().x / tile_size)) * tile_size;
		tile_sprite.setTextureRect(sf::IntRect(tileset_x, tileset_y, tile_size, tile_size));
		tile_sprite.setPosition(tile.getGridPosition().x * tile_size, tile.getGridPosition().y * tile_size);
		image.draw(tile_sprite);
	}
}
