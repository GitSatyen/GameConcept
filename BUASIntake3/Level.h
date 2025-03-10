#pragma once
#include "LDtkLoader/Project.hpp"
#include "SFML/Graphics/Image.hpp"
class Level
{
public:

	Level() = default;
	void loadFile(const std::string& filepath);
	void draw(sf::RenderTarget& image);

private:
	ldtk::Project project;
	ldtk::World world;
	ldtk::Level level;

	sf::Sprite tile_sprite;
	sf::Texture tileset_texture;
};

	
