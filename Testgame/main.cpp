#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace sf;
const float PI = 3.14159265f;

void printMatrix(const float* ptr) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << ptr[i * 4 + j] << " ";
		}
		std::cout << std::endl;
	}
}



class Bullet
{
public:
	float speed;
	Vector2f velocity;
	CircleShape shape;

	Bullet(): speed(0.6), velocity(0,0)
	{
		shape.setFillColor(Color::Black);
		shape.setRadius(2.5);
	}
};

int main()
{
	bool pause = 0;
	float moveSpeed = 0.3;
	Vector2f playerCenter, mousePos, aimDir, aimDirNorm, bulStartPos;
	std::vector<Bullet> bullets;
	std::vector<RectangleShape> walls(5);
	Bullet bul;
	RectangleShape wall;
	Clock clock;
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
	herotext.loadFromImage(hero);
	
	Sprite player;
	player.setTexture(herotext);
	player.setTextureRect(IntRect(0,18,62,92));
	player.setPosition(400, 300);
	player.setOrigin(31, 46);

	srand(time(NULL));
	for (auto it = walls.begin(); it != walls.end(); it++) {
		it->setFillColor(Color::Blue);
		it->setPosition(rand() % 800, rand() % 600);
		it->setSize(Vector2f(rand() % 100 + 20, rand() % 40 + 10));
	}

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();

		fps.setString(std::to_string((int)time));

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

		playerCenter = Vector2f(player.getPosition());
		bulStartPos = Vector2f(playerCenter.x, playerCenter.y - 46);
		mousePos = Vector2f(Mouse::getPosition(window));
		aimDir = mousePos - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI + 90;
		player.setRotation(deg);

		deg = (deg + 30) * PI / 180;
		bulStartPos = Vector2f(playerCenter.x + (bulStartPos.x - playerCenter.x)*std::cos(deg) - (bulStartPos.y - playerCenter.y) * std::sin(deg),
			playerCenter.y + (bulStartPos.x - playerCenter.x) * std::sin(deg) + (bulStartPos.y - playerCenter.y) * std::cos(deg));

		FloatRect frect =  player.getGlobalBounds();
		RectangleShape rect(Vector2f(frect.width, frect.height));
		rect.setFillColor(Color(255,255,255,0));
		rect.setPosition(frect.left, frect.top);
		rect.setOutlineColor(Color::Red);
		rect.setOutlineThickness(1);

		if (Mouse::isButtonPressed(Mouse::Left)) {
			bul.shape.setPosition(bulStartPos.x-2.5, bulStartPos.y-2.5);
			bul.velocity = bul.speed * time * aimDirNorm;

			bullets.push_back(bul);
		}

		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].shape.move(bullets[i].velocity);

			Vector2f bulPos = bullets[i].shape.getPosition();
			if (bulPos.x < 0 || bulPos.y < 0 || bulPos.x > window.getSize().x || bulPos.y > window.getSize().y) {
				bullets.erase(bullets.begin() + i);
			}
			else {
				FloatRect bound;
				bound = bullets[i].shape.getGlobalBounds();
				for (int j = 0; j < walls.size(); j++) {
					if (bound.intersects(walls[j].getGlobalBounds())) {
						Vector2f sideVec, sideOrtoVec, bulPos, vel, sideOrtoVecNorm;
						float tmp;
						bulPos = bullets[i].shape.getPosition();
						if (bulPos.x > walls[j].getPosition().x)
							if (bulPos.y > walls[j].getPosition().y)
								if (bulPos.x > walls[j].getPosition().x + walls[j].getSize().x)
									sideVec = Vector2f(0, walls[j].getSize().y);
								else
									sideVec = Vector2f(walls[j].getSize().x, 0);
							else
								sideVec = Vector2f(walls[j].getSize().x, 0);
						else
							sideVec = Vector2f(0, walls[j].getSize().y);

						sideOrtoVec.x = sideVec.y;
						sideOrtoVec.y = -sideVec.x;
						
						sideOrtoVecNorm = sideOrtoVec / sqrt(pow(sideOrtoVec.x, 2) + pow(sideOrtoVec.y, 2));
						tmp = bullets[i].velocity.x * sideOrtoVecNorm.x + bullets[i].velocity.y * sideOrtoVecNorm.y;

						vel = bullets[i].velocity - sideOrtoVecNorm * 2.f * tmp;
						
						bullets[i].velocity.x = vel.x;
						bullets[i].velocity.y = vel.y;
						break;
					}
				}
			}
		}

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

		window.clear(Color(255, 255,255));
		
		for(int i = 0; i < bullets.size(); i++)
			window.draw(bullets[i].shape);

		for (int i = 0; i < walls.size(); i++)
			window.draw(walls[i]);

		//window.draw(rect);
		window.draw(player);
		window.draw(fps);
		window.display();
	}

	return 0;
}