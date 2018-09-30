#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Wall: public Entity, public sf::RectangleShape
{
	void draw(sf::RenderWindow& win);
};