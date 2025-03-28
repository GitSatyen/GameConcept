#include "Entity.h"

Entity::Entity(const sf::Vector2f& pos)
{
}

void Entity::setPosition(const sf::Vector2f& pos)
{
	position = pos;
}

const sf::Vector2f Entity::getPosition()
{
	return position;
}
