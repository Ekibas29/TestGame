#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player: public Entity, public sf::Sprite
{
	sf::FloatRect collisionBox;
public:
	Player(sf::Texture& texture);

	void draw(sf::RenderWindow& win);
	sf::FloatRect getBox();
	void move();
	void setPosition(float x, float y);
	void resetVelocity();
};