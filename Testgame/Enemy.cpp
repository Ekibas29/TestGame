#include "Enemy.h"

Enemy::Enemy(sf::Texture& texture) : Player(texture) {
	setPosition(700, 500);
};

void Enemy::RayCasting(sf::RenderWindow& win) {
	float angle = getRotation();
	float speed = 5;
	int cnt = 30;
	sf::CircleShape shape;
	shape.setFillColor(sf::Color::Red);
	shape.setRadius(1.0f);
	shape.setPosition(getPosition());

	sf::Vector2f playerCenter = getPosition();
	sf::Vector2f bulStartPos;

	for (float delta = -30; delta <= 30; delta += 1.0f) {
		cnt = 30;
		angle = getRotation();
		shape.setPosition(getPosition());

		angle = (angle + delta) * PI / 180;

		bulStartPos = sf::Vector2f(playerCenter.x, playerCenter.y - 55);
		bulStartPos = sf::Vector2f(playerCenter.x + (bulStartPos.x - playerCenter.x)*std::cos(angle) - (bulStartPos.y - playerCenter.y) * std::sin(angle),
			playerCenter.y + (bulStartPos.x - playerCenter.x) * std::sin(angle) + (bulStartPos.y - playerCenter.y) * std::cos(angle));

		sf::Vector2f aimDir = bulStartPos - playerCenter;
		sf::Vector2f aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		sf::Vector2f vel = aimDirNorm * speed;

		while (cnt) {
			shape.move(vel);
			win.draw(shape);
			cnt--;
		}
	}

}