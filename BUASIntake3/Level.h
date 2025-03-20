#pragma once
#include "LDtkLoader/Project.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics.hpp"
class Level
{
public:
	
	//std::string readFile(const std::string& filepath);
	Level(const std::string& filepath);
	void draw(sf::RenderTarget& image);
	const ldtk::World& getWorld() const { return world; }
	const ldtk::Level& getLevel() const { return level; }
	

private:
	ldtk::Project project;
	const ldtk::World& world; //Using pointers because World can't be default contructed
	const ldtk::Level& level; //Using pointers because Level can't be default contructed
	ldtk::Project loadProject(const std::string& filepath);

	sf::Sprite tile_sprite;
	sf::Texture tileset_texture;
};

	
