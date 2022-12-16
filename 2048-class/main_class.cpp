#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string> //new(12/15)
#include <math.h> // new(12/15)
#include "menu.h"
#include "game.h"

//#include <windows.h>
bool g_win = false;
bool g_start = false;
bool g_lose = false;

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(4*64-8, 5*64-8), "2048");

    Menu menu(window.getSize().x, window.getSize().y);
    Game game(window.getSize().x, window.getSize().y);

    //固定的事件循環邏輯
    while (window.isOpen())
    {   
        //sf::Event => 產生一些與視窗互動的事件(例如：點擊、鍵盤...)
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    //////在初始畫面//////
                    if (!g_start) 
                    {   
                        int game_mode = 0;
                        if (event.key.code == sf::Keyboard::Up)
                            menu.MoveUp();
                        else if (event.key.code == sf::Keyboard::Down)
                            menu.MoveDown();
                        else if (event.key.code == sf::Keyboard::Return)
                        {
                            g_start = true;

                        }
                    }
                    //////進入遊戲畫面//////
                    else if (g_start && !g_win)
                    {
                        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                            game.shift(Direction::Left);
                        else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                            game.shift(Direction::Right);
                        else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                            game.shift(Direction::Up);
                        else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                            game.shift(Direction::Down);
                    } 
                    //////贏了//////
                    else if (g_start && g_win)
                    {

                    }
                    //////輸了//////
                    else if (g_start && g_lose)
                    {

                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        ////////// CHECK IF USER WON THE GAME //////////
        /*
        if (!g_win)
            for (int i=0; i<N; i++)
                for (int j=0; j<M; j++)
                    if (MAP[i][j] == 2049)
                    {
                        g_win = 1;
                        break;
                    }
        */
        ////////// DRAW //////////
        window.clear(); //畫新的刪舊的
    
        if (!g_start) 
            menu.draw(window);
        else if (g_start && !g_win)
            game.draw(window);

        window.display(); //show出來
    }

    return 0;
}
