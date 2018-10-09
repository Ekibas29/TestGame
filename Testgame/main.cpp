#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "Bullet.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"

using namespace sf;
const float delay = 0.1f;
const float todeg = 180.0 / PI;
const int wallsNumber = 5;

void printMatrix(const float* ptr) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << ptr[i * 4 + j] << " ";
		}
		std::cout << std::endl;
	}
}

float getAngle(Vector2f v1, Vector2f v2) {
	Vector2f v1Norm, v2Norm;
	v1Norm = v1 / sqrt(pow(v1.x, 2) + pow(v1.y, 2));
	v2Norm = v2 / sqrt(pow(v2.x, 2) + pow(v2.y, 2));

	return acos(v1Norm.x * v2Norm.x + v1Norm.y * v2Norm.y) * todeg;
}

int main()
{
	bool pause = 0;
	float moveSpeed = 0.3f;
	Vector2f playerCenter, mousePos, aimDir, aimDirNorm, bulStartPos;
	std::vector<Bullet> bullets;
	std::vector<Wall> walls;
	Bullet bul;
	Clock clock, mouseClock;
	Text fps;
	Font font;

	font.loadFromFile("font.ttf");
	fps.setOutlineColor(Color::Green);
	fps.setFillColor(Color::Red);
	fps.setCharacterSize(20);
	fps.setFont(font);

	RenderWindow window(VideoMode(800, 600), "TestGame");
	window.setFramerateLimit(60);
	
	Image hero, icon;
	hero.loadFromFile("player.png");
	icon.loadFromFile("icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Texture herotext;
	herotext.loadFromImage(hero, sf::IntRect(91, 18, 62, 87));
	Player player(herotext);
	Enemy enemy(herotext);


	std::cout << enemy.getRotation() << std::endl;
	/******Init walls*****/
	srand(time(NULL));
	std::vector<Wall>::const_iterator it;
	for (int i = 0; i < wallsNumber; i++) {
		Wall wall(rand() % 700, rand() % 500, rand() % 100 + 50, rand() % 50 + 30);
		for (it = walls.begin(); it != walls.end(); it++) {
			if (wall.getGlobalBounds().intersects(it->getGlobalBounds())) {
				i--;
				break;
			}
		}
		if (it == walls.end() && !wall.getGlobalBounds().intersects(player.getBox()) 
			&& !wall.getGlobalBounds().intersects(enemy.getBox()))
			walls.push_back(wall);
	}

	std::vector<Vector2f> normVec(4);
	normVec[0] = Vector2f(0, 1);
	normVec[1] = Vector2f(1, 0);
	normVec[2] = Vector2f(0, -1);
	normVec[3] = Vector2f(-1, 0);

	/*
	sf::ConvexShape convex;
	convex.setPointCount(3);
	convex.setFillColor(Color::Red);
	convex.setPoint(0, sf::Vector2f(500, 250));
	convex.setPoint(1, sf::Vector2f(600, 350));
	convex.setPoint(2, sf::Vector2f(400, 350));
	*/

	while (window.isOpen())
	{
		int time = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		fps.setString(std::to_string(time));

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == sf::Event::LostFocus)
				pause = 1;
			if (event.type == sf::Event::GainedFocus)
				pause = 0;
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
					if(pause) pause = 0;
					else {
						pause = 1;
						fps.setString("PAUSE");
						window.clear(Color(255, 255, 255));

						for (int i = 0; i < bullets.size(); i++) {
							window.draw(bullets[i].shape);
							//window.draw(bullets[i].rect);
						}

						for (int i = 0; i < walls.size(); i++)
							window.draw(walls[i]);
						enemy.RayCasting(window);
						//window.draw(rect);
						player.draw(window);
						window.draw(fps);

						window.display();
					}
		}

		if (pause) continue;
		
		/*****Player rotation*****/
		playerCenter = player.getPosition();
		bulStartPos = Vector2f(playerCenter.x, playerCenter.y - 55);
		mousePos = Vector2f(Mouse::getPosition(window));
		aimDir = mousePos - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI + 90;
		player.setRotation(deg);
		enemy.setRotation(deg);

		std::cout << enemy.getRotation() << std::endl;

		deg = (deg + 30) * PI / 180;
		bulStartPos = Vector2f(playerCenter.x + (bulStartPos.x - playerCenter.x)*std::cos(deg) - (bulStartPos.y - playerCenter.y) * std::sin(deg),
			playerCenter.y + (bulStartPos.x - playerCenter.x) * std::sin(deg) + (bulStartPos.y - playerCenter.y) * std::cos(deg));

		/*Vector2f aimPosDir = mousePos - bulStartPos;
		Vector2f aimPosDirNorm = aimPosDir / sqrt(pow(aimPosDir.x, 2) + pow(aimPosDir.y, 2));
		player.setRotation(atan2(aimPosDirNorm.y, aimPosDirNorm.x) * 180 / PI);
		std::cout << "Angle: " << getAngle(aimDir, mousePos - bulStartPos) << std::endl;
		*/

		/****Display player bounds****/
		FloatRect frect =  player.getBox();
		RectangleShape rect(Vector2f(frect.width, frect.height));
		rect.setFillColor(Color(255,255,255,0));
		rect.setPosition(frect.left, frect.top);
		rect.setOutlineColor(Color::Red);
		rect.setOutlineThickness(1);

		/******Shooting******/
		if (Mouse::isButtonPressed(Mouse::Left) && mouseClock.getElapsedTime().asSeconds() > delay) {
			mouseClock.restart();
			bul.setPosition(bulStartPos.x-2.5f, bulStartPos.y-2.5f);
			bul.velocity = bul.speed * aimDirNorm;
			for (int j = 0; j < walls.size(); j++) {
				if (bul.center.intersects(walls[j].getGlobalBounds()))
					break;
				else if(j == walls.size() - 1)
					bullets.push_back(bul);
			}
		}

		/******Bullets movement and collision******/
		for (int i = 0; i < bullets.size(); i++) {
			Vector2f prevBulPos = Vector2f(bullets[i].center.left, bullets[i].center.top);
			bullets[i].move();

			Vector2f currBulPos = Vector2f(bullets[i].center.left, bullets[i].center.top);
			if (currBulPos.x < 0 || currBulPos.y < 0 || currBulPos.x > window.getSize().x || currBulPos.y > window.getSize().y) {
				bullets.erase(bullets.begin() + i);
			}
			else {
				for (int j = 0; j < walls.size(); j++) {
					if (bullets[i].center.intersects(walls[j].getGlobalBounds())) {
						Vector2f sideOrtoVec, sideOrtoVecNorm, vel, normvel;
						float angle, tmp;
						normvel = bullets[i].velocity / sqrt(pow(bullets[i].velocity.x, 2) + pow(bullets[i].velocity.y, 2));
						
						for (int k = 0; k < normVec.size(); k++) {
							angle = acos(normvel.x*normVec[k].x + normvel.y*normVec[k].y) * todeg;
							if (0 <= angle || angle <= 90) {
								tmp = bullets[i].velocity.x * normVec[k].x + bullets[i].velocity.y * normVec[k].y;
								vel = bullets[i].velocity - normVec[k] * 2.f * tmp;

								bul = bullets[i];
								bul.velocity.x = vel.x;
								bul.velocity.y = vel.y;
								bul.move();
								if (!bul.center.intersects(walls[j].getGlobalBounds())) {
									bullets[i].velocity.x = vel.x;
									bullets[i].velocity.y = vel.y;
									break;
								}
							}
						}
						break;
					}
				}
			}
		}

		/******Player movement******/
		player.resetVelocity();
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			player.velocity.x = player.velocity.x - moveSpeed*time;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			player.velocity.x = player.velocity.x + moveSpeed * time;
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			player.velocity.y = player.velocity.y - moveSpeed*time;
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			player.velocity.y = player.velocity.y + moveSpeed * time;
		}

		if (player.velocity.x && player.velocity.y) {
			player.velocity *= 0.707106f;
		}
		player.move();
		//std::cout << sqrt(pow(player.velocity.x, 2) + pow(player.velocity.y, 2)) << std::endl;

		for (int j = 0; j < walls.size(); j++) {
			if (player.getBox().intersects(walls[j].getGlobalBounds())) {
				if (Keyboard::isKeyPressed(Keyboard::A)) {
					player.velocity.x = player.velocity.x + (-2)*player.velocity.x;
				}
				if (Keyboard::isKeyPressed(Keyboard::D)) {
					player.velocity.x = player.velocity.x - 2*player.velocity.x;
				}
				if (Keyboard::isKeyPressed(Keyboard::W)) {
					player.velocity.y = player.velocity.y + (-2)*player.velocity.y;
				}
				if(Keyboard::isKeyPressed(Keyboard::S)) {
					player.velocity.y = player.velocity.y - 2*player.velocity.y;
				}
				player.move();
			}
		}

		Vector2f playerUpLeftPos, playerBotRightPos;
		playerUpLeftPos = Vector2f(player.getBox().left, player.getBox().top);
		playerBotRightPos = Vector2f(player.getBox().left + player.getBox().width,
			player.getBox().top + player.getBox().height);

		if (playerUpLeftPos.x < 0)
			player.setPosition(player.getPosition().x + -playerUpLeftPos.x, player.getPosition().y);
		if (playerUpLeftPos.y < 0)
			player.setPosition(player.getPosition().x, player.getPosition().y + -playerUpLeftPos.y);
		if (playerBotRightPos.x > window.getSize().x)
			player.setPosition(player.getPosition().x - (playerBotRightPos.x - window.getSize().x), player.getPosition().y);
		if (playerBotRightPos.y > window.getSize().y)
			player.setPosition(player.getPosition().x, player.getPosition().y - (playerBotRightPos.y - window.getSize().y));

		/******Display objects on screen******/
		window.clear(Color(255, 255,255));
		
		enemy.RayCasting(window);

		for (int i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].shape);
			//window.draw(bullets[i].rect);
		}

		for (int i = 0; i < walls.size(); i++)
			window.draw(walls[i]);

		//window.draw(rect);
		player.draw(window);
		enemy.draw(window);
		window.draw(fps);
		//window.draw(convex);

		window.display();
	}

	return 0;
}