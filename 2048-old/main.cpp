#include <SFML/Graphics.hpp>
#include "Game.h"
#include "menu.h"

int main()
{
	srand(time(NULL));

	sf::RenderWindow wnd(sf::VideoMode(600, 800), "2048", sf::Style::Titlebar | sf::Style::Close);
	sf::Event e;

	wnd.setKeyRepeatEnabled(false);
	mn::Menu menu(wnd.getSize().x, wnd.getSize().y);
	gm::Game game(wnd.getSize().x, wnd.getSize().y);

	while (wnd.isOpen()) {
		while (wnd.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				wnd.close();
			
			menu.onEvent(e, wnd, game);
		}
		if (menu.isOn()) 
            menu.draw(wnd);
		else
		{
			game.Update();
			wnd.clear(sf::Color(187, 173, 160));
			game.Render(wnd);
		}
		wnd.display();
	}


	return 0;
}

