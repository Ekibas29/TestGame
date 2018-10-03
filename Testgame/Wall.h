#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Wall: public Entity, public sf::RectangleShape
{
public:
	const sf::Vector2f leftTop;
	const sf::Vector2f rightTop;
	const sf::Vector2f leftBot;
	const sf::Vector2f rightBot;

	Wall(float left, float top, float width, float height);
	void draw(sf::RenderWindow& win);
};