#pragma once
#include <SFML/Graphics.hpp>

const int MAX_ITEMS = 3;

class Menu
{
    private:
        int selectItemsIndex = 0;
        sf::Font font;
        sf::Text menu[MAX_ITEMS];
    public:
        Menu(float width, float height);
        ~Menu();

        void draw(sf::RenderWindow &window);
        void MoveUp();
        void MoveDown();
        int getSelectItemsIndex();
};
