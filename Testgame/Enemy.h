#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Enemy : public Player
{
public:
	Enemy(sf::Texture& texture);
	void RayCasting(sf::RenderWindow& win);

};