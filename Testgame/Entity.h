#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
	float speed;
	sf::Vector2f velocity;
	
	virtual void draw(sf::RenderWindow&) = 0;
	virtual ~Entity() {};
};