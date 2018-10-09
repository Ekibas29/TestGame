#pragma once
#include <SFML/Graphics.hpp>

const float PI = 3.14159265f;

class Entity {
public:
	float speed;
	sf::Vector2f velocity;
	
	virtual void draw(sf::RenderWindow&) = 0;
	virtual ~Entity() {};
};