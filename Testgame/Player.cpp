#include "Player.h"

Player::Player(sf::Texture& texture) {
	setTexture(texture);
	setPosition(400, 300);
	setOrigin(31, 46);
}

void Player::draw(sf::RenderWindow& win) {
	win.draw(*this);
}