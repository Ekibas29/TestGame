#include "Bullet.h"

Bullet::Bullet() : speed(0.6f), velocity(0, 0) {
	shape.setFillColor(sf::Color::Black);
	shape.setRadius(2.5f);

	rect.setSize(sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height));
	rect.setFillColor(sf::Color(255, 255, 255, 0));
	rect.setOutlineColor(sf::Color::Red);
	rect.setOutlineThickness(1);
	center.height = 1.f;
	center.width = 1.f;
}

void Bullet::setPosition(float x, float y) {
	shape.setPosition(x, y);
	rect.setPosition(shape.getGlobalBounds().left, shape.getGlobalBounds().top);
	center.left = shape.getPosition().x + shape.getRadius();
	center.top = shape.getPosition().y + shape.getRadius();
}

void Bullet::move() {
	shape.move(velocity);
	rect.move(velocity);
	center.left = shape.getPosition().x + shape.getRadius();
	center.top = shape.getPosition().y + shape.getRadius();
}
void Bullet::draw(sf::RenderWindow& win) {
	win.draw(shape);
}