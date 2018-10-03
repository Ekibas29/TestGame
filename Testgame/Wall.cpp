#include "Wall.h"

Wall::Wall(float left, float top, float width, float height): 
	leftTop(left, top), rightTop(left + width, top),
	leftBot(left, top + height), rightBot(left + width, top + height)
{
	setFillColor(sf::Color::Blue);
	setPosition(left, top);
	setSize(sf::Vector2f(width, height));
}

void Wall::draw(sf::RenderWindow& win) {
	win.draw(*this);
}