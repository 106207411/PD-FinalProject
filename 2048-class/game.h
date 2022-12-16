#pragma once

#include <SFML/Graphics.hpp>
#include "setting.h"

const int MAX_POWER = 15;

class Tile
{
private:
    sf::Vector2i pos;
    sf::Vector2i prevPos;
    int value;
    bool animaMove;
public:
    Tile();
    ~Tile();
    friend class Game;      
};

class Game
{
    private:
        sf::Texture picture[MAX_POWER+1]; //sf::Texture => 是一張圖片，但主要是用來改變物件的外觀
        sf::Sprite picture_with_position[MAX_POWER+1]; //sf::Sprite => 是一張圖片，同時也是一個物件，意思是他擁有座標(x, y)
        sf::Vector2i genPos();
        int genNewTile();
        int cnt = 0;
        //Tile MAP[M][N];
    public:
        int map[M][N] = {0};// map initialization
        Game(float width, float height);
        ~Game();

        void shift(Direction d);
        void draw(sf::RenderWindow &window);
        void placeNewTile();
        void randomEvent(); 
        void coutMap();
        int getCnt(){return cnt;};
        void setCnt(){cnt++;}
        bool isGameOver();  // Check if the game is over
        void reset();       // Reset the game
};