#include "Entity.h"

Entity::Entity(const sf::Vector2f& pos)
{
}

void Entity::setPosition(const sf::Vector2f& pos)
{
	transform.translate(pos);
}

const sf::Vector2f Entity::getPosition()
{
	return sf::Vector2f();
}
