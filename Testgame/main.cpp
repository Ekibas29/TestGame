#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "Bullet.h"
#include "Player.h"
#include "Wall.h"

using namespace sf;
const float PI = 3.14159265f;
const float delay = 0.1f;
const float todeg = 180.0 / PI;

void printMatrix(const float* ptr) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << ptr[i * 4 + j] << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	bool pause = 0;
	float moveSpeed = 0.3f;
	Vector2f playerCenter, mousePos, aimDir, aimDirNorm, bulStartPos;
	std::vector<Bullet> bullets;
	std::vector<Wall> walls(5);
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
	herotext.loadFromImage(hero, sf::IntRect(0, 18, 62, 92));
	Player player(herotext);

	/******Init walls*****/
	srand(time(NULL));
	for (auto it = walls.begin(); it != walls.end(); it++) {
		it->setFillColor(Color::Blue);
		it->setPosition(rand() % 700, rand() % 500);
		it->setSize(Vector2f(rand() % 100 + 50, rand() % 50 + 30));
	}

	std::vector<Vector2f> normVec(4);
	normVec[0] = Vector2f(0, 1);
	normVec[1] = Vector2f(1, 0);
	normVec[2] = Vector2f(0, -1);
	normVec[3] = Vector2f(-1, 0);

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
					else pause = 1;
		}

		if (pause) continue;
		
		/*****Поворот персонажа в сторону курсора*****/
		playerCenter = player.getPosition();
		bulStartPos = Vector2f(playerCenter.x, playerCenter.y - 46);
		mousePos = Vector2f(Mouse::getPosition(window));
		aimDir = mousePos - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI + 90;
		player.setRotation(deg);

		deg = (deg + 30) * PI / 180;
		bulStartPos = Vector2f(playerCenter.x + (bulStartPos.x - playerCenter.x)*std::cos(deg) - (bulStartPos.y - playerCenter.y) * std::sin(deg),
			playerCenter.y + (bulStartPos.x - playerCenter.x) * std::sin(deg) + (bulStartPos.y - playerCenter.y) * std::cos(deg));

		/****Отображение границ персонажа****/
		FloatRect frect =  player.getGlobalBounds();
		RectangleShape rect(Vector2f(frect.width, frect.height));
		rect.setFillColor(Color(255,255,255,0));
		rect.setPosition(frect.left, frect.top);
		rect.setOutlineColor(Color::Red);
		rect.setOutlineThickness(1);

		/******Стрельба******/
		if (Mouse::isButtonPressed(Mouse::Left) && mouseClock.getElapsedTime().asSeconds() > delay) {
			mouseClock.restart();
			bul.setPosition(bulStartPos.x-2.5f, bulStartPos.y-2.5f);
			bul.velocity = bul.speed * time * aimDirNorm;

			bullets.push_back(bul);
		}

		/******Движение и коллизия пуль******/
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



		/******Движение персонажа******/
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			player.move(-moveSpeed*time, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			player.move(moveSpeed*time, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			player.move(0, -moveSpeed*time);
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			player.move(0, moveSpeed*time);
		}

		/******Вывод объектов на экран******/
		window.clear(Color(255, 255,255));
		
		for (int i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].shape);
			//window.draw(bullets[i].rect);
		}

		for (int i = 0; i < walls.size(); i++)
			window.draw(walls[i]);

		//window.draw(rect);
		Entity& ptr = player;
		ptr.draw(window);
		window.draw(fps);
		window.display();
	}

	return 0;
}