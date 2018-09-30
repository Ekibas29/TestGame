#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Bullet: public Entity
{
public:
	float speed;
	sf::Vector2f velocity;
	sf::CircleShape shape;
	sf::RectangleShape rect;
	sf::FloatRect center;

	Bullet();

	void setPosition(float x, float y);
	void move();
	void draw(sf::RenderWindow& win);
};