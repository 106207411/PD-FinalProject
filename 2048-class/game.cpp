#include "game.h"
#include <math.h> 
#include <iostream>

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

    //initialize map
    /*
    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++)
        {   
            map[i][j].value = 0;
            map[i][j].pos.x = i;
            map[i][j].pos.y = j;
            map[i][j].prevPos.x = i;
            map[i][j].prevPos.y = j;
            map[i][j].animaMove = false;
        }
    */
   
    //initial generate two tiles 
    this->placeNewTile();
    this->placeNewTile();
}

Game::~Game()
{

}
void Game::draw(sf::RenderWindow &window) //渲染視窗
{
    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++)
        {   
            int index = 0;
            while (true)
            {   
                if (map[i][j] == 0)
                {
                    index = 0;
                    break;
                }
                else if (map[i][j] == pow(2, index))
                    break;
                index++;
            }
            picture_with_position[index].setPosition(j*TILESIZE, i*TILESIZE+TILESIZE);
            window.draw(picture_with_position[index]);
        }
}
void Game::shift(Direction d) //控制移動事件
{
    bool didShift = 0;
    if (d == Direction::Up)
    {
        for (int j=0; j<M; j++) // Every row
        {
            for (int i=0; i<N-1; i++) // Search for every 0 (empty tile) and remove it
                if (map[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=i; k<N-1; k++) // Shift everything up
                    {
                        map[k][j] = map[k+1][j];
                        if (map[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    map[N-1][j] = 0;
                    if (change) i--; // If something really changed (did not shift only 0s) check again the current position
                }
                // Merge the tiles with the same number
            for (int i=0; i<N-1; i++) // Merge the tiles with the same number
                if (map[i][j] == map[i+1][j] && map[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=i; k<N-1; k++)
                        map[k][j] = map[k+1][j];
                    map[N-1][j] = 0;
                    map[i][j] *= 2;
                }
        }
    }
    else if (d == Direction::Left)
    {
        for (int i=0; i<N; i++) // Every line
        {
            for (int j=0; j<M-1; j++) // Search for every 0 (empty tile) and remove it
                if (map[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=j; k<M-1; k++) // Shift everything up
                    {
                        map[i][k] = map[i][k+1];
                        if (map[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    map[i][M-1] = 0;
                    if (change) j--; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int j=0; j<N-1; j++) // Merge the tiles with the same number
                if (map[i][j] == map[i][j+1] && map[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=j; k<M-1; k++)
                        map[i][k] = map[i][k+1];
                    map[i][M-1] = 0;
                    map[i][j] *= 2;
                }
        }
    }
    else if (d == Direction::Down)
    {
        for (int j=0; j<M; j++) // Every row
        {
            for (int i=N-1; i>=0; i--){ // Search for every 0 (empty tile) and remove it
                if (map[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=i; k>0; k--) // Shift everything down
                    {
                        map[k][j] = map[k-1][j];
                        if (map[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    map[0][j] = 0; //下移的話，最上面那麼會補0
                    if (change) i++; // If something really changed (did not shift only 0s) check again the current position
                    //***當移好，要補i回去，讓他繼續在同個位置往上做事
                }
            }
            for (int i=N-1; i>0; i--) // Merge the tiles with the same number
                if (map[i][j] == map[i-1][j] && map[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=i; k>=0; k--)
                        map[k][j] = map[k-1][j];
                    map[0][j] = 0;
                    map[i][j] *= 2;
                }
        }
    }
    else if (d == Direction::Right)
    {
        for (int i=0; i<N; i++) // Every line
        {
            for (int j=M-1; j>=0; j--) // Search for every 0 (empty tile) and remove it
                if (map[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=j; k>0; k--) // Shift everything to the right
                    {
                        map[i][k] = map[i][k-1];
                        if (map[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    map[i][0] = 0;
                    if (change) j++; // If something really changed (did not shift only 0s) check again the current position
                }
            for (int j=M-1; j>0; j--) // Merge the tiles with the same number
                if (map[i][j] == map[i][j-1] && map[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=j; k>0; k--)
                        map[i][k] = map[i][k-1];
                    map[i][0] = 0;
                    map[i][j] *= 2;
                }
        }
    }
    //移完之後的行動
    if (didShift){
        this->placeNewTile();
        this->setCnt();
        if (this->getCnt()%5 == 0)
            this->randomEvent();
    }

    // Check if the game is over
    if (this->isGameOver())
    {
        std::cout << "Game Over!" << std::endl;
        this->reset();
    }

}
sf::Vector2i Game::genPos() //隨機生成tiles => position的觀點，確認該地點為0
{
    sf::Vector2i v;
    while(1)
    {
        v.x = rand()%N;
        v.y = rand()%N;
        if (map[v.y][v.x] == 0)
            break;
    }
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
    map[p.y][p.x] = this->genNewTile();
}
void Game::randomEvent() //隨機事件
{
    int eventcode = rand()%3;
    if (eventcode == 0) //randomEvent1: 將數字變大*2
    {
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                if ( map[i][j] == pow(2, this->getCnt()%5+1) ) 
                {
                    map[i][j] =  map[i][j]*2;
                }
            }
        }
    }
    else if (eventcode == 1)//randomEvent2: 消掉2
    {
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                if ( map[i][j] == 2 ) 
                {
                    map[i][j] =  0;
                }
            }
        }
    }
    else if (eventcode == 2) //randomEvent3: 將數字變小/2
    {
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                if ( map[i][j] == pow(2, this->getCnt()%5+2) ) 
                {
                    map[i][j] =  map[i][j]/2;
                }
            }
        }
    }
}
void Game::coutMap() //COUT
{
     for (int a=0; a<N; a++)
        {
            for (int b=0; b<M; b++)
                std::cout << map[a][b] << " ";
            std::cout << "\n";
        }
    std::cout << "\n";
}

bool Game::isGameOver() 
{
    for (int i=0; i<N; i++)
        for (int j=0; j<M; j++) 
        {
            if (map[i][j] == 0)
                return 0;
            if (i>0 && map[i][j] == map[i-1][j])
                return 0;
            if (i<N-1 && map[i][j] == map[i+1][j])
                return 0;
            if (j>0 && map[i][j] == map[i][j-1])
                return 0;
            if (j<M-1 && map[i][j] == map[i][j+1])
                return 0;
        }
    return 1;
}

// TBD
void Game::reset()
{
    for (int i=0; i<N; i++)
        for (int j=0; j<M; j++)
            map[i][j] = 0;
    this->placeNewTile();
    this->placeNewTile();
    this->setCnt();
}