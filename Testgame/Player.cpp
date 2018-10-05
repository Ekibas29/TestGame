#include "Player.h"

Player::Player(sf::Texture& texture) {
	setTexture(texture);
	setPosition(40, 50);
	setOrigin(27, 60);
	//setScale(0.5f, 0.5f);
	collisionBox.width = 41;
	collisionBox.height = 41;
	collisionBox.left = getPosition().x - 21;
	collisionBox.top = getPosition().y - 20;
}

void Player::draw(sf::RenderWindow& win) {
	win.draw(*this);
}

void Player::move() {
	Sprite::move(velocity);
	collisionBox.left = getPosition().x - 21;
	collisionBox.top = getPosition().y - 20;
}

void Player::setPosition(float x, float y) {
	Sprite::setPosition(x, y);
	collisionBox.left = getPosition().x - 21;
	collisionBox.top = getPosition().y - 20;
}

void Player::resetVelocity() {
	velocity.x = 0; 
	velocity.y = 0;
}

sf::FloatRect Player::getBox() {
	return collisionBox;
}