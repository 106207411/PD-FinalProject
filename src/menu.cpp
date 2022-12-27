#include <iostream>
#include "Game.h"
#include "menu.h"

namespace mn{

    Menu::Menu(float width, float height)
    {
        if (!font.loadFromFile("TaipeiSansTCBeta-Bold.ttf"))
        {
            std::cout << "can't load TaipeiSansTCBeta-Bold.ttf" << std::endl;
        }

        menu[0].setFont(font);
        menu[0].setFillColor(sf::Color(143,123,102));
        menu[0].setString(L"2048原始版");
        menu[0].setCharacterSize(55);
        sf::FloatRect fr0 = menu[0].getLocalBounds();
        menu[0].setPosition(sf::Vector2f((width - fr0.width) / 2 ,height / (MAX_ITEMS + 1 ) * 1));

        menu[1].setFont(font);
        menu[1].setFillColor(sf::Color::White);
        menu[1].setString(L"併校模擬器");
        menu[1].setCharacterSize(55);
        sf::FloatRect fr1 = menu[1].getLocalBounds();
        menu[1].setPosition(sf::Vector2f((width - fr1.width) / 2, height / (MAX_ITEMS + 1) * 2));

        menu[2].setFont(font);
        menu[2].setFillColor(sf::Color::White);
        menu[2].setString(L"動物森友會");
        menu[2].setCharacterSize(55);
        sf::FloatRect fr2 = menu[2].getLocalBounds();
        menu[2].setPosition(sf::Vector2f((width - fr2.width) / 2,height / (MAX_ITEMS + 1 ) * 3));

        menu[3].setFont(font);
        menu[3].setFillColor(sf::Color::White);
        menu[3].setString("Exit");
        menu[3].setCharacterSize(55);
        sf::FloatRect fr3 = menu[3].getLocalBounds();
        menu[3].setPosition(sf::Vector2f((width - fr3.width) / 2,height / (MAX_ITEMS + 1 ) * 4));
    }

    Menu::~Menu()
    {
    }
    void Menu::draw(sf::RenderWindow &window)
    {   
        window.clear(sf::Color(187, 173, 160));
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
            menu[selectItemsIndex].setFillColor(sf::Color(143,123,102));   
        }
    }
    void Menu::MoveDown()
    {
        if (selectItemsIndex+1 < MAX_ITEMS)
        {
            menu[selectItemsIndex].setFillColor(sf::Color::White);
            selectItemsIndex++;
            menu[selectItemsIndex].setFillColor(sf::Color(143,123,102)); 
        }
    }
    void Menu::onEvent(sf::Event e, sf::RenderWindow &wnd, gm::Game &game)
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Up)
                MoveUp();
            else if (e.key.code == sf::Keyboard::Down)
                MoveDown();
            else if (e.key.code == sf::Keyboard::Return)
            {
                int mode = getSelectItemsIndex();
                (mode == 3) ? wnd.close() : game.setMode(mode);
                setMenuState(false);
            }
        }
    }
    int Menu::getSelectItemsIndex(){
        return selectItemsIndex;
    }
    bool Menu::isOn(){
        return menuOn;
    }
    void Menu::setMenuState(bool on){
        menuOn = on;
    }
}
///////Menu END////////