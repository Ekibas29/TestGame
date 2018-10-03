#include "Player.h"

Player::Player(sf::Texture& texture) {
	setTexture(texture);
	setPosition(40, 50);
	setOrigin(31, 46);
	setScale(0.5f, 0.5f);
}

void Player::draw(sf::RenderWindow& win) {
	win.draw(*this);
}