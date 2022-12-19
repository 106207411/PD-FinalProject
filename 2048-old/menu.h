#pragma once
#include <SFML/Graphics.hpp>

namespace mn
{
    const int MAX_ITEMS = 3;
    class Menu
    {
        private:
            int selectItemsIndex = 0;
            bool menuOn = true;
            sf::Font font;
            sf::Text menu[MAX_ITEMS];
        public:
            Menu(float width, float height);
            ~Menu();

            void draw(sf::RenderWindow &window);
            void MoveUp();
            void MoveDown();
            void onEvent(sf::Event e, sf::RenderWindow &wnd, gm::Game &game);
            int getSelectItemsIndex();
            bool isOn();
    };
}