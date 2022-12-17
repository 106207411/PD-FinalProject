#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	srand(time(NULL));

	sf::RenderWindow wnd(sf::VideoMode(600, 800), "2048", sf::Style::Titlebar | sf::Style::Close);
	sf::Event e;

	wnd.setKeyRepeatEnabled(false);

	tw::Game game(wnd.getSize().x, wnd.getSize().y);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				wnd.close();
			game.OnEvent(e);
		}

		game.Update();

		wnd.clear(sf::Color(187, 173, 160));
		game.Render(wnd);
		wnd.display();
	}


	return 0;
}

