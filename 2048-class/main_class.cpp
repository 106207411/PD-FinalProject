#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string> //new(12/15)
#include <math.h> // new(12/15)
//#include <Menu.h>
//#include <windows.h>
const int N = 4, M = 4,
    TILESIZE = 64,
    WIDTH = N*TILESIZE,
    HEIGHT = M*TILESIZE+TILESIZE;
bool g_win = false;
bool g_start = false;
bool g_lose = false;
int MAP[N][M];
int cnt = 0; //回合計數
enum class Direction
{
    Left,
    Right,
    Up,
    Down,
};

///////Game////////
const int MAX_POWER = 15;

class Game
{
private:
    sf::Texture picture[MAX_POWER+1]; //sf::Texture => 是一張圖片，但主要是用來改變物件的外觀
    sf::Sprite picture_with_position[MAX_POWER+1]; //sf::Sprite => 是一張圖片，同時也是一個物件，意思是他擁有座標(x, y)
    sf::Vector2i genPos();
    int genNewTile();
public:
    Game(float width, float height);
    ~Game();

    void shift(Direction d);
    void draw(sf::RenderWindow &window);
    void placeNewTile();
    void randomEvent(int cnt); 
    void coutMap();
};

Game::Game(float width, float height) //備好資料
{   
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
                if (MAP[i][j] == 0)
                {
                    index = 0;
                    break;
                }
                else if (MAP[i][j] == pow(2, index))
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
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=i; k<N-1; k++) // Shift everything up
                    {
                        MAP[k][j] = MAP[k+1][j];
                        if (MAP[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[N-1][j] = 0;
                    if (change) i--; // If something really changed (did not shift only 0s) check again the current position
                }
                // Merge the tiles with the same number
                else if (MAP[i][j] == MAP[i+1][j] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=i; k<N-1; k++)
                        MAP[k][j] = MAP[k+1][j];
                    MAP[N-1][j] = 0;
                    MAP[i][j] *= 2;
                }
        }
    }
    else if (d == Direction::Left)
    {
        for (int i=0; i<N; i++) // Every line
        {
            for (int j=0; j<M-1; j++) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=j; k<M-1; k++) // Shift everything up
                    {
                        MAP[i][k] = MAP[i][k+1];
                        if (MAP[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[i][M-1] = 0;
                    if (change) j--; // If something really changed (did not shift only 0s) check again the current position
                }
                // Merge the tiles with the same number
                else if (MAP[i][j] == MAP[i][j+1] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=j; k<M-1; k++)
                        MAP[i][k] = MAP[i][k+1];
                    MAP[i][M-1] = 0;
                    MAP[i][j] *= 2;
                }
        }
    }
    else if (d == Direction::Down)
    {
        for (int j=0; j<M; j++) // Every row
        {
            for (int i=N-1; i>=0; i--){ // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=i; k>0; k--) // Shift everything down
                    {
                        MAP[k][j] = MAP[k-1][j];
                        if (MAP[k][j] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[0][j] = 0; //下移的話，最上面那麼會補0
                    if (change) i++; // If something really changed (did not shift only 0s) check again the current position
                    //***當移好，要補i回去，讓他繼續在同個位置往上做事
                }
                else if (MAP[i][j] == MAP[i-1][j] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=i; k>=0; k--)
                        MAP[k][j] = MAP[k-1][j];
                    MAP[0][j] = 0;
                    MAP[i][j] *= 2;
                }
            }
        }
    }
    else if (d == Direction::Right)
    {
        for (int i=0; i<N; i++) // Every line
        {
            for (int j=M-1; j>=0; j--) // Search for every 0 (empty tile) and remove it
                if (MAP[i][j] == 0)
                {
                    bool change = 0;
                    for (int k=j; k>0; k--) // Shift everything to the right
                    {
                        MAP[i][k] = MAP[i][k-1];
                        if (MAP[i][k] != 0)
                        {
                            didShift = 1;
                            change = 1;
                        }
                    }
                    MAP[i][0] = 0;
                    if (change) j++; // If something really changed (did not shift only 0s) check again the current position
                }
                // Merge the tiles with the same number
                else if (MAP[i][j] == MAP[i][j-1] && MAP[i][j] != 0)
                {
                    didShift = 1;
                    for (int k=j; k>0; k--)
                        MAP[i][k] = MAP[i][k-1];
                    MAP[i][0] = 0;
                    MAP[i][j] *= 2;
                }
        }
    }
    //移完之後的行動
    if (didShift){
        this->placeNewTile();
        cnt++;
        if (cnt%5 == 0)
            this->randomEvent(cnt);
    }
}

//隨機生成tiles => position的觀點，確認該地點為0
sf::Vector2i Game::genPos()
{
    sf::Vector2i v;
    while(1)
    {
        v.x = rand()%N;
        v.y = rand()%N;
        if (MAP[v.y][v.x] == 0)
            break;
    }
    return v;
}

//隨機生成數字，2或4
int Game::genNewTile()
{
    int t = rand()%2;
    if (t==0)
        return 2;
    else
        return 4;
}

//給定數字，先隨機產生地點(為0)，再給數字(2 or 4)
void Game::placeNewTile()
{
    sf::Vector2i p = this->genPos();
    MAP[p.y][p.x] = this->genNewTile();
}

void Game::randomEvent(int cnt) //隨機事件
{
    int eventcode = rand()%3;
    if (eventcode == 0) //randomEvent1: 將數字變大*2
    {
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                if ( MAP[i][j] == pow(2, cnt%5+1) ) 
                {
                    MAP[i][j] =  MAP[i][j]*2;
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
                if ( MAP[i][j] == 2 ) 
                {
                    MAP[i][j] =  0;
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
                if ( MAP[i][j] == pow(2, cnt%5+2) ) 
                {
                    MAP[i][j] =  MAP[i][j]/2;
                }
            }
        }
    }
}
//COUT
void Game::coutMap()
{
     for (int a=0; a<N; a++)
        {
            for (int b=0; b<M; b++)
                std::cout << MAP[a][b] << " ";
            std::cout << "\n";
        }
    std::cout << "\n";
}

///////Game END////////

///////Menu////////
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
};

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
///////Menu END////////

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(WIDTH-8, HEIGHT-8), "2048");

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
                            g_start = true;
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
        if (!g_win)
            for (int i=0; i<N; i++)
                for (int j=0; j<M; j++)
                    if (MAP[i][j] == 2049)
                    {
                        g_win = 1;
                        break;
                    }

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
