#include "game.h"
#include <math.h> 
#include <iostream>

//MAP[i][j] = MAP[WIDTH_PIXEL][HEIGHT_PIXEL]
Tile::Tile()
{            
    value = 0;
    pos.x = 0;
    pos.y = 0;
    prevPos.x = 0;
    prevPos.y = 0;
    animaMove = false;
}

void Tile::deletePos()
{
    this->pos.x = 0;
    this->pos.y = 0;
    this->prevPos.x = 0;
    this->prevPos.y = 0;
}

Tile::~Tile()
{

}

Game::Game(float width, float height) //備好資料
{   
    //load picture
    picture[0].loadFromFile("images/1.png");
    picture[1].loadFromFile("images/2.png");
    picture[2].loadFromFile("images/4.png");
    picture[3].loadFromFile("images/8.png");
    picture[4].loadFromFile("images/16.png");
    picture[5].loadFromFile("images/32.png");
    picture[6].loadFromFile("images/64.png");
    picture[7].loadFromFile("images/128.png");
    picture[8].loadFromFile("images/256.png");
    picture[9].loadFromFile("images/512.png");
    picture[10].loadFromFile("images/1024.png");
    picture[11].loadFromFile("images/2048.png");

    for (int i=0; i<= MAX_POWER; i++){
        //std::string index = std::to_string(pow(2, i));
        //picture[i].loadFromFile("images/0.png");
        picture_with_position[i].setTexture(picture[i]);
    }

    //initialize MAP
    for (int i=0; i<WIDTH_PIXEL; i++)
        for (int j=0; j<HEIGHT_PIXEL; j++)
        {   
            MAP[i][j].value = 0;
            MAP[i][j].pos.x = i;
            MAP[i][j].pos.y = j;
            MAP[i][j].prevPos.x = i;
            MAP[i][j].prevPos.y = j;
            MAP[i][j].animaMove = false;
        }

   
    //initial generate two tiles 
    this->placeNewTile();
    this->placeNewTile();
}

Game::~Game()
{

}
void Game::draw(sf::RenderWindow &window) //渲染視窗
{
    for (int i=0; i<WIDTH_PIXEL; i++)
        for (int j=0; j<HEIGHT_PIXEL; j++)
        {   
            int index = 0;
            while (true)
            {   
                if (MAP[i][j].value == 0)
                {
                    index = 0;
                    break;
                }
                else if (MAP[i][j].value == pow(2, index))
                    break;
                index++;
            }
            picture_with_position[index].setPosition(i*TILESIZE, j*TILESIZE+TILESIZE);
            window.draw(picture_with_position[index]);
        }
}

void Game::shift(Direction d) //控制移動事件
{
    bool is_merge = false;
    bool didShift = false;
    if (d == Direction::Up)
    {
        for (int x=0; x<WIDTH_PIXEL; x++)
        {   
            for (int y=0; y<HEIGHT_PIXEL-1; y++)
            {   
                sf::Vector2i finalPos = sf::Vector2i(x, y);
                for (int y_move=y+1; y_move<HEIGHT_PIXEL; y_move++)
                {
                    if (MAP[x][y_move].value != 0){
                        if (MAP[x][y].value != 0)
                            if (MAP[x][y_move].value != MAP[x][y].value)
                                break;
                            else 
                                is_merge = true;
                        tileMove(sf::Vector2i(x, y_move), finalPos, is_merge);
                        didShift = true;
                        if (!is_merge) y--;
                        is_merge = false;
                        break;
                    }
                }
            }
        }
    }
    else if (d == Direction::Left)
    {
        for (int y=0; y<HEIGHT_PIXEL; y++)
        {   
            for (int x=0; x<WIDTH_PIXEL-1; x++)
            {
                sf::Vector2i finalPos = sf::Vector2i(x, y);
                for (int x_move=x+1; x_move<HEIGHT_PIXEL; x_move++) 
                {
                    if (MAP[x_move][y].value != 0)
                    {
                        if (MAP[x][y].value != 0)
                            if (MAP[x_move][y].value != MAP[x][y].value)
                                break;
                            else 
                                is_merge = true;
                        tileMove(sf::Vector2i(x_move, y), finalPos, is_merge);
                        didShift = true;
                        if (!is_merge) x--;
                        is_merge = false;
                        break;
                    }
                }
            }
        }
    }
    else if (d == Direction::Down)
    {
        for (int x=0; x<WIDTH_PIXEL; x++)
        {   
            for (int y=HEIGHT_PIXEL-1; y>0; y--)
            {   
                sf::Vector2i finalPos = sf::Vector2i(x, y);

                for (int y_move=y-1; y_move>=0; y_move--)
                {
                    if (MAP[x][y_move].value != 0)
                    {
                        if (MAP[x][y].value != 0)
                            if (MAP[x][y_move].value != MAP[x][y].value)
                                break;
                            else 
                                is_merge = true;
                        tileMove(sf::Vector2i(x, y_move), finalPos, is_merge);
                        didShift = true;
                        if (!is_merge) y++;
                        is_merge = false;
                        break;
                    }
                }
            }
        }
    }
    else if (d == Direction::Right)
    {
        for (int y=0; y<HEIGHT_PIXEL; y++)
        {   
            for (int x=HEIGHT_PIXEL-1; x>0; x--)
            {   
                sf::Vector2i finalPos = sf::Vector2i(x, y);
                for (int x_move=x-1; x_move>=0; x_move--)
                {
                    if (MAP[x_move][y].value != 0)
                    {
                        if (MAP[x][y].value != 0)
                            if (MAP[x_move][y].value != MAP[x][y].value)
                                break;
                            else 
                                is_merge = true;
                        tileMove(sf::Vector2i(x_move, y), finalPos, is_merge);
                        didShift = true;
                        if (!is_merge) x++;
                        is_merge = false;
                        break;
                    }
                }
            }
        }
    }
    if (didShift){
        this->placeNewTile();
        this->setCnt();
        std::cout << cnt << "\n";
        coutMap();
        /*if (this->getCnt()%5 == 0)
            this->randomEvent();*/
    }
}

void Game::tileMove(sf::Vector2i i, sf::Vector2i f, bool is_merge)
{
    std::cout << "from: " << i.x << ", " << i.y << "\n" << "value: "<<MAP[i.x][i.y].value<<"\n";
    std::cout << "to: " << f.x << ", " << f.y << "\n"<< "value: "<<MAP[f.x][f.y].value<<"\n";
    //std::cout << i.x << ", " << i.y << "\n";
    MAP[f.x][f.y].value = (is_merge) ? MAP[i.x][i.y].value*2 : MAP[i.x][i.y].value;
    MAP[i.x][i.y].value = 0;
    MAP[i.x][i.y].deletePos();
    coutMap();
}

sf::Vector2i Game::genPos() //隨機生成tiles => position的觀點，確認該地點為0
{
    sf::Vector2i v;
    while(1)
    {
        v.x = rand()%WIDTH_PIXEL;
        v.y = rand()%HEIGHT_PIXEL;
        if (MAP[v.y][v.x].value == 0)
            break;
    }
    MAP[v.y][v.x].pos = v;
    MAP[v.y][v.x].prevPos = MAP[v.y][v.x].pos;
    return v;
}
int Game::genNewTile() //隨機生成數字，2或4
{
    int t = rand()%2;
    return (t==0) ? 2: 4;
}
void Game::placeNewTile()//給定數字，先隨機產生地點(為0)，再給數字(2 or 4)
{
    sf::Vector2i p = this->genPos();
    MAP[p.y][p.x].value = this->genNewTile();
}
void Game::randomEvent() //隨機事件
{
    int eventcode = rand()%3;
    if (eventcode == 0) //randomEvent1: 將數字變大*2
    {
        for (int i=0; i<WIDTH_PIXEL; i++)
        {
            for (int j=0; j<HEIGHT_PIXEL; j++)
            {
                if ( MAP[i][j].value == pow(2, this->getCnt()%5+1) ) 
                {
                    MAP[i][j].value =  MAP[i][j].value*2;
                }
            }
        }
    }
    else if (eventcode == 1)//randomEvent2: 消掉2
    {
        for (int i=0; i<WIDTH_PIXEL; i++)
        {
            for (int j=0; j<HEIGHT_PIXEL; j++)
            {
                if ( MAP[i][j].value == 2 ) 
                {
                    MAP[i][j].value =  0;
                }
            }
        }
    }
    else if (eventcode == 2) //randomEvent3: 將數字變小/2
    {
        for (int i=0; i<WIDTH_PIXEL; i++)
        {
            for (int j=0; j<HEIGHT_PIXEL; j++)
            {
                if ( MAP[i][j].value == pow(2, this->getCnt()%5+2) ) 
                {
                    MAP[i][j].value =  MAP[i][j].value/2;
                }
            }
        }
    }
}
void Game::coutMap() //COUT
{
     for (int a=0; a<WIDTH_PIXEL; a++)
        {
            for (int b=0; b<HEIGHT_PIXEL; b++)
                std::cout << MAP[b][a].value << " ";
            std::cout << "\n";
        }
    std::cout << "\n";
}
