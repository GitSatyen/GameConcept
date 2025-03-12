#pragma once
#include "LDtkLoader/Project.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics.hpp"
class Level
{
public:

	Level(const std::string& filepath);
	void draw(sf::RenderTarget& image);

private:
	ldtk::Project project;
	const ldtk::World& world; //Using pointers because World can't be default contructed
	const ldtk::Level& level; //Using pointers because Level can't be default contructed

	sf::Sprite tile_sprite;
	sf::Texture tileset_texture;
};

	
