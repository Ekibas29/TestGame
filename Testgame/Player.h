#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player: public Entity, public sf::Sprite
{
public:
	Player(sf::Texture& texture);

	void draw(sf::RenderWindow& win);
};