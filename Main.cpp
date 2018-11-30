#include "Game.h"

int main(int argc, char* argv[])
{
	sf::RenderWindow window(sf::VideoMode(1935, 1080), "2D Shooter", sf::Style::Close);

	sf::Clock clock;
	float deltaTime = 0.f;

	Game game(&window);

	while (window.isOpen())
	{
		sf::Event evnt;

		while (window.pollEvent(evnt))
		{
			if (evnt.type == sf::Event::Closed)
				window.close();
			if (evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::Escape)
				window.close();
		}
		deltaTime = clock.restart().asSeconds();

		game.update(deltaTime);
		
		game.draw();
	}
	
	return 0;
}