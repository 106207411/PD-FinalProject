#include <iostream>
#include "menu.h"

Menu::Menu(float width, float height)
{
    if (!font.loadFromFile("Arial.ttf"))
    {
        std::cout << "can't load font" << std::endl;
    }

    menu[0].setFont(font);
    menu[0].setFillColor(sf::Color::Red);
    menu[0].setString("Play");
    menu[0].setCharacterSize(20);
    menu[0].setPosition(sf::Vector2f(width/2,height/(MAX_ITEMS+1)*1));

    menu[1].setFont(font);
    menu[1].setFillColor(sf::Color::White);
    menu[1].setString("Play Hard");
    menu[1].setCharacterSize(20);
    menu[1].setPosition(sf::Vector2f(width/2,height/(MAX_ITEMS+1)*2));

    menu[2].setFont(font);
    menu[2].setFillColor(sf::Color::White);
    menu[2].setString("Exit");
    menu[2].setCharacterSize(20);
    menu[2].setPosition(sf::Vector2f(width/2,height/(MAX_ITEMS+1)*3));
}

Menu::~Menu()
{
}
void Menu::draw(sf::RenderWindow &window)
{
    for (int i=0; i<MAX_ITEMS; i++)
    {
        window.draw(menu[i]);
    }
}
void Menu::MoveUp()
{
    if (selectItemsIndex-1 >= 0)
    {
        menu[selectItemsIndex].setFillColor(sf::Color::White);
        selectItemsIndex--;
        menu[selectItemsIndex].setFillColor(sf::Color::Red);   
    }
}
void Menu::MoveDown()
{
    if (selectItemsIndex+1 < MAX_ITEMS)
    {
        menu[selectItemsIndex].setFillColor(sf::Color::White);
        selectItemsIndex++;
        menu[selectItemsIndex].setFillColor(sf::Color::Red);   
    }
}
int Menu::getSelectItemsIndex(){
    return selectItemsIndex;
}
///////Menu END////////