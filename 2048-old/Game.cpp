#include "Game.h"
#include <deque>
#include <unordered_map>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>

int count = 0;  // 計算回合數

namespace gm
{
    Game::Game(int w, int h)
    {
        tileSize = (w - FIELD_MARGIN * 2 - TILE_MARGIN * (FIELD_WIDTH - 1)) / FIELD_WIDTH;
        headerSize = ( h - FIELD_MARGIN*2 - (TILE_MARGIN + tileSize) * (FIELD_HEIGHT));
        std::cout << "headerSize: " << headerSize << "\n";
        animState = false;
        chanceYes = false;
        destinyYes = false;

        Reset();
    }

    void Game::setMode(int mode)
    {
        this->mode = mode;
        if (!font.loadFromFile("./TaipeiSansTCBeta-Bold.ttf"))
            printf("Failed to load TaipeiSansTCBeta-Bold.ttf\n");
    }
    
    int Game::getMode()
    {
        return this->mode;
    }

    void Game::OnEvent(sf::Event e)
    {
        if (e.type == sf::Event::KeyPressed)
        {
            if (animState)
                return;

            sf::Keyboard::Key kc = e.key.code;

            if (kc == sf::Keyboard::Left || kc == sf::Keyboard::A)
                move(-1, 0);
            else if (kc == sf::Keyboard::Right || kc == sf::Keyboard::D)
                move(1, 0);
            else if (kc == sf::Keyboard::Up || kc == sf::Keyboard::W)
                move(0, -1);
            else if (kc == sf::Keyboard::Down || kc == sf::Keyboard::S)
                move(0, 1);
        }
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            // if mouse position is in the RectangleShape restart Button
            if (restartButton.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
            {
                Reset();
            }
        }
    }

    void Game::Update()
    {
        if (animState)
        {
            if (animClock.getElapsedTime().asSeconds() >= ANIMATION_DURATION)
            {
                animState = false;

                for (int i = 0; i < moves.size(); i++)
                {
                    sf::Vector2i f = moves[i].first.first;
                    sf::Vector2i t = moves[i].first.second;

                    char srcVal = moves[i].second;
                    char destVal = map[t.x][t.y];

                    map[f.x][f.y] = 0;

                    if (srcVal == destVal && f != t && srcVal != 12 && srcVal != 13)
                            map[t.x][t.y] = srcVal + 1;
                    else
                            map[t.x][t.y] = srcVal ;
                }

                moves.clear();
                
                
                // 機會：所有數字乘2，且場上所有機會、命運的都會消失
                if ( chanceYes )
                {
                    for (int x = 0; x < FIELD_WIDTH; x++)
                    {
                        for (int y = 0; y < FIELD_HEIGHT; y++)
                        {
                            if ( map[x][y] != 0 && map[x][y] != 12 &&  map[x][y] != 13 )
                            {
                                map[x][y] += 1;
                                tempMap[x][y] += 1;
                            }
                            if ( map[x][y] == 12 || map[x][y] == 13 )
                                map[x][y] = 0;
                        }
                    }
                }
                chanceYes = false;
                
                // 命運：所有數字除 2 or 炸彈 (清空所有數字)，且場上所有的機會、命運都會消失
                if ( destinyYes )
                {
                    int t = rand() ;
                    if ( t % 20 == 0 ) // 炸彈，清空所有空格 (機率 5%)
                    {
                        for (int x = 0; x < FIELD_WIDTH; x++)
                        {
                            for (int y = 0; y < FIELD_HEIGHT; y++)
                            {
                                    map[x][y] = 0;
                            }
                        }
                    }
                    else if ( t % 20 == 1 || t % 20 == 2 || t % 20 == 3 || t % 20 == 4 )
                        // 所有數字乘 4 (機率 20%)
                    {
                        for (int x = 0; x < FIELD_WIDTH; x++)
                        {
                            for (int y = 0; y < FIELD_HEIGHT; y++)
                            {
                                if ( map[x][y] < 9 )
                                    map[x][y] += 2;
                                else if ( map[x][y] == 12 || map[x][y] == 13 )
                                    map[x][y] = 0;
                            }
                        }
                    }
                    else // 所有數字除 2（若數字為2則變成空白）(機率 75%)
                    {
                        for (int x = 0; x < FIELD_WIDTH; x++)
                        {
                            for (int y = 0; y < FIELD_HEIGHT; y++)
                            {
                                if ( map[x][y] != 0 && map[x][y] < 10 ) //(10是1024)
                                {
                                    map[x][y] -= 1;
                                    tempMap[x][y] -= 1;
                                }
                                else if ( map[x][y] == 12 || map[x][y] == 13 )
                                    map[x][y] = 0;
                            }
                        }
                    }
                    
                }
                destinyYes = false;
                
                Spawn();

                return;
            }


        }
    }

    void Game::Render(sf::RenderTarget& tgt)
    {
        sf::Text text;
        text.setFont(font);
        text.setStyle(sf::Text::Style::Bold);
        
        //header
        sf::RectangleShape title;
        title.setSize(sf::Vector2f( tileSize * 4 + TILE_MARGIN * 3, (headerSize / 2) - TILE_MARGIN));
        title.setFillColor(sf::Color(187, 173, 160));
        title.setPosition( FIELD_MARGIN , FIELD_MARGIN );
        tgt.draw(title);

        text.setString((getMode() == 0) ? L"2048原始版" : L"併校模擬器"); //L"GAME 2048"
        sf::FloatRect fr = text.getLocalBounds();
        text.setPosition( FIELD_MARGIN , FIELD_MARGIN );
        text.setCharacterSize(55);
        text.setFillColor(sf::Color(143,123,102));
        tgt.draw(text);

        sf::RectangleShape score_board;
        score_board.setSize(sf::Vector2f( tileSize * 2 + TILE_MARGIN, (headerSize / 2) - TILE_MARGIN));
        score_board.setFillColor(sf::Color(187, 173, 160));
        score_board.setPosition( FIELD_MARGIN , FIELD_MARGIN + (headerSize / 2) );
        tgt.draw(score_board);

        text.setString("2048");
        text.setPosition( FIELD_MARGIN , FIELD_MARGIN + (headerSize / 2) );
        text.setFillColor(sf::Color(255, 255, 255));
        tgt.draw(text);

        sf::RectangleShape restart;
        restart.setSize(sf::Vector2f( tileSize * 1.5 + TILE_MARGIN , (headerSize / 2) - (TILE_MARGIN * 2) ));
        restart.setFillColor(sf::Color(143,123,102));
        restart.setPosition( FIELD_MARGIN + tileSize * 2.5 + TILE_MARGIN * 2, TILE_MARGIN * 3 + (headerSize / 2) );
        this->restartButton = restart;
        tgt.draw(restart);

        text.setString(L"重新開始");
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Style::Regular);
        sf::FloatRect textRect = text.getLocalBounds();
        //text.setOrigin((tileSize * 1.5 + TILE_MARGIN - textRect.width) / 2 + textRect.left, ((headerSize / 2) - (TILE_MARGIN * 2) - textRect.height) / 2 + textRect.top);
        text.setPosition( FIELD_MARGIN + tileSize * 2.5 + TILE_MARGIN * 2 + (tileSize * 1.5 + TILE_MARGIN - textRect.width) / 2 + textRect.left , TILE_MARGIN * 3 + (headerSize / 2) + ((headerSize / 2) - (TILE_MARGIN * 2) - textRect.height) / 2);
        text.setFillColor(sf::Color(255, 255, 255));
        tgt.draw(text);

        //main
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(tileSize, tileSize));
        
        for (int x = 0; x < FIELD_WIDTH; x++) {
            for (int y = 0; y < FIELD_HEIGHT; y++) {
                tile.setPosition(FIELD_MARGIN + x * (tileSize + TILE_MARGIN), headerSize + FIELD_MARGIN + y * (tileSize + TILE_MARGIN));
                tile.setFillColor(getTileColor(map[x][y]));
                
                tgt.draw(tile);

                if (map[x][y] != 0) {
                    text.setCharacterSize(getTextSize(map[x][y]));
                    text.setString(getText(map[x][y]));
                    text.setStyle(sf::Text::Style::Regular);
                    sf::FloatRect fr = text.getLocalBounds();

                    text.setPosition(FIELD_MARGIN + x * (tileSize + TILE_MARGIN) + (tileSize - fr.width) / 2 - fr.left, headerSize + FIELD_MARGIN + y * (tileSize + TILE_MARGIN) + (tileSize - fr.height) / 2 - fr.top);
                    text.setFillColor(getTextColor(map[x][y]));

                    tgt.draw(text);
                }
            }
        }

        sf::Vector2i fMargin(FIELD_MARGIN, FIELD_MARGIN);

        for (int i = 0; i < moves.size(); i++)
        {
            sf::Vector2i orig = moves[i].first.first;
            sf::Vector2i f = fMargin + orig * (tileSize + TILE_MARGIN);
            sf::Vector2i t = fMargin + moves[i].first.second * (tileSize + TILE_MARGIN);

            float movePercent = (animClock.getElapsedTime().asSeconds() / ANIMATION_DURATION);
            sf::Vector2f curPos = sf::Vector2f(f) + sf::Vector2f(t - f) * movePercent;
            curPos.y += headerSize;

            text.setCharacterSize(getTextSize(moves[i].second));
            text.setString(getText(moves[i].second));

            sf::FloatRect fr = text.getLocalBounds();

            text.setPosition(curPos.x + (tileSize - fr.width) / 2 - fr.left, curPos.y + (tileSize - fr.height) / 2 - fr.top);
            text.setFillColor(getTextColor(moves[i].second));

            tile.setPosition(curPos);
            tile.setFillColor(getTileColor(moves[i].second));

            tgt.draw(tile);
            tgt.draw(text);
        }
    }

    void Game::Spawn()
    {
        int availableCount = 0;
        sf::Vector2i availableMoves[FIELD_WIDTH*FIELD_HEIGHT];

        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            for (int y = 0; y < FIELD_HEIGHT; y++)
            {
                if (map[x][y] == 0)
                {
                    availableMoves[availableCount] = sf::Vector2i(x, y);
                    availableCount++;
                }
            }
        }

        if (availableCount == 0)
        {
            Reset();
            return;
        }

        sf::Vector2i newPos = availableMoves[rand() % availableCount];
        char newTileID;
        if ( count % 50 == 0 && count != 0 )  // 每 50 回合會發一張機會/命運
            newTileID = (rand() % 2) == 0 ? 12 : 13;
        else
            newTileID = (rand() % 10) < 9 ? 1 : 2;
        
        map[newPos.x][newPos.y] = newTileID;
        count++ ;
    }

    void Game::Reset()
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
            for (int y = 0; y < FIELD_HEIGHT; y++)
                map[x][y] = 0;
        count = 0;
        Spawn();
    }

    sf::Color Game::getTileColor(char tile)
    {
        static const sf::Color colors[] =
        {
            sf::Color(238, 228, 218, 97),        // empty
            sf::Color(238, 228, 218),            // 2^1 == 2
            sf::Color(237, 224, 200),            // 2^2 == 4
            sf::Color(242, 177, 121),            // 2^3 == 8
            sf::Color(245, 149, 99),            // 2^4 == 16
            sf::Color(246, 124, 95),            // 2^5 == 32
            sf::Color(246, 94, 59),                // 2^6 == 64
            sf::Color(237, 207, 114),            // 2^7 == 128
            sf::Color(237, 204, 97),            // 2^8 == 256
            sf::Color(237, 200, 80),            // 2^9 == 512
            sf::Color(237, 197, 63),            // 2^10 == 1024
            sf::Color(237, 194, 46),            // 2^11 == 2048
            sf::Color(240, 128, 128),            // 機會(12)
            sf::Color(112, 128, 144)            // 命運(13)
            
        };
        return colors[tile];
    }

    sf::Color Game::getTextColor(char tile)
    {
        if (tile >= 3) // tile >= 8 (cuz 2^3 == 8)
            return sf::Color(249, 246, 242);
        return sf::Color(119, 110, 101);
    }

    std::wstring Game::getText(char tile)
    {
        // this is small optimization - we dont have to use pow()
        static const std::wstring text[14] = {
            L"",                // empty
            L"2",            // 2^1 == 2
            L"4",            // 2^2 == 4
            L"8",            // 2^3 == 8
            L"16",            // 2^4 == 16
            L"32",            // 2^5 == 32
            L"64",            // 2^6 == 64
            L"128",            // 2^7 == 128
            L"256",            // 2^8 == 256
            L"512",            // 2^9 == 512
            L"1024",            // 2^10 == 1024
            L"2048",            // 2^11 == 2048
            L"機會",
            L"命運"
        };

        // For chinese text, you need to use std:::wstring
        // https://www.sfml-dev.org/tutorials/2.5/graphics-text.php
        static const std::wstring school[14] = {
            L"",
            L"萬能科大",
            L"華夏科大",
            L"真理大學",
            L"台灣大學",
            L"台灣首府大學",
            L"亞洲大學",
            L"北科",
            L"虎科",
            L"雲科",
            L"台科",
            L"大Ya大學",
            L"機會",
            L"命運"
        };

        return (mode==0) ? text[tile] : school[tile];
    }

    int Game::getTextSize(char tile)  // 格子內的字體大小
    {
        if (mode==0)
        {
            if (tile >= 10)
                return 35;
            else if (tile >= 7)
                return 45;
            return 55;
        }
        else if (mode==1)
        {
            return 25;
        }
    }



    void Game::move(char dirX, char dirY)
    {   
        // copy map
        for (int x = 0; x < FIELD_WIDTH; x++)
            for (int y = 0; y < FIELD_HEIGHT; y++)
                tempMap[x][y] = map[x][y];

        bool is_merge = false;
        bool didShift = false;

        if (dirY == -1) //上
        {
            for (int x=0; x<FIELD_WIDTH; x++)
            {   
                for (int y=0; y<FIELD_HEIGHT-1; y++)
                {   
                    sf::Vector2i finalPos = sf::Vector2i(x, y);
                    for (int y_move=y+1; y_move<FIELD_HEIGHT; y_move++)
                    {
                        if (tempMap[x][y_move] != 0){
                            if (tempMap[x][y] != 0)
                                if (tempMap[x][y_move] != tempMap[x][y])
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
        else if (dirX == -1) // 左
        {
            for (int y=0; y<FIELD_HEIGHT; y++)
            {   
                for (int x=0; x<FIELD_WIDTH-1; x++)
                {
                    sf::Vector2i finalPos = sf::Vector2i(x, y);
                    for (int x_move=x+1; x_move<FIELD_WIDTH; x_move++) 
                    {
                        if (tempMap[x_move][y] != 0)
                        {
                            if (tempMap[x][y] != 0)
                                if (tempMap[x_move][y] != tempMap[x][y])
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
        else if (dirY == 1) //下
        {
            for (int x=0; x<FIELD_WIDTH; x++)
            {   
                for (int y=FIELD_HEIGHT-1; y>0; y--)
                {   
                    sf::Vector2i finalPos = sf::Vector2i(x, y);

                    for (int y_move=y-1; y_move>=0; y_move--)
                    {
                        if (tempMap[x][y_move] != 0)
                        {
                            if (tempMap[x][y] != 0)
                                if (tempMap[x][y_move] != tempMap[x][y])
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
        else if (dirX == 1) // 右
        {
            for (int y=0; y<FIELD_HEIGHT; y++)
            {   
                for (int x=FIELD_WIDTH-1; x>0; x--)
                {   
                    sf::Vector2i finalPos = sf::Vector2i(x, y);
                    for (int x_move=x-1; x_move>=0; x_move--)
                    {
                        if (tempMap[x_move][y] != 0)
                        {
                            if (tempMap[x][y] != 0)
                                if (tempMap[x_move][y] != tempMap[x][y])
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
    
        if (didShift){ //移完之後的行動
        }

        bool isFilled = true;
        bool isGameOver = true;
        for (int x = 0; x < FIELD_WIDTH; x++) {
            for (int y = 0; y < FIELD_HEIGHT; y++) {
                if (tempMap[x][y] == 0)
                {
                    isFilled = false;
                    isGameOver = false;
                    break;
                }

                char val = tempMap[x][y];

                if (x != 0 && tempMap[x - 1][y] == val)
                    isGameOver = false;
                else if (y != 0 && tempMap[x][y - 1] == val)
                    isGameOver = false;
                else if (x != FIELD_WIDTH - 1 && tempMap[x + 1][y] == val)
                    isGameOver = false;
                else if (y != FIELD_HEIGHT - 1 && tempMap[x][y+1] == val)
                    isGameOver = false;
            }

            if (!isFilled || !isGameOver)
                break;
        }

        if (isGameOver)
            Reset();   
    }

    void Game::tileMove(sf::Vector2i f, sf::Vector2i t, bool is_merge)
    {
        char srcVal = tempMap[f.x][f.y];
        char destVal = tempMap[t.x][t.y];
        tempMap[t.x][t.y] = (is_merge) ? srcVal+1 : srcVal;
        tempMap[f.x][f.y] = 0;
        if ( f != t )
        {
            if ( destVal == 12 )
            {
                chanceYes = true;
                map[f.x][f.y] = 0;
                tempMap[f.x][f.y] = 0;
                map[t.x][t.y] = 0;
                tempMap[t.x][t.y] = 0;
            }
            if ( destVal == 13 )
            {
                destinyYes = true;
                map[f.x][f.y] = 0;
                tempMap[f.x][f.y] = 0;
                map[t.x][t.y] = 0;
                tempMap[t.x][t.y] = 0;
            }
            else if ( destVal != 12 &&  destVal != 13 )
            {
                tempMap[t.x][t.y] = srcVal + 1;
                if (srcVal + 1 == 12 ) Reset();
            }

            map[f.x][f.y] = 0;
            moves.push_back(std::make_pair(std::make_pair(f, t), srcVal));

            animState = true;
            animClock.restart();
        }
    }


    // void Game::applyMove(sf::Vector2i f, sf::Vector2i t, int dx, int dy)
    // {
    //     char srcVal = tempMap[f.x][f.y];
    //     char destVal = tempMap[t.x][t.y];

    //     tempMap[f.x][f.y] = 0;

    //     if (destVal == srcVal)   // 若兩位置上的值一樣
    //     {
    //         if ( destVal == 12 )
    //         {
    //             chanceYes = true;
    //             map[f.x][f.y] = 0;
    //             tempMap[f.x][f.y] = 0;
    //             map[t.x][t.y] = 0;
    //             tempMap[t.x][t.y] = 0;
    //         }
    //         if ( destVal == 13 )
    //         {
    //             destinyYes = true;
    //             map[f.x][f.y] = 0;
    //             tempMap[f.x][f.y] = 0;
    //             map[t.x][t.y] = 0;
    //             tempMap[t.x][t.y] = 0;
    //         }
    //         else if ( destVal != 12 &&  destVal != 13 )
    //         {
    //             tempMap[t.x][t.y] = srcVal + 1;
    //             if (srcVal + 1 == 12 ) Reset();
    //         }
    //     }
    //     else
    //         tempMap[t.x - (dx * (destVal != 0))][ t.y - (dy * (destVal != 0))] = srcVal;

    //     sf::Vector2i from = f, to;

    //     if (destVal == srcVal)
    //         to = t;
    //     else
    //         to = sf::Vector2i(t.x - (dx * (destVal != 0)), t.y - (dy * (destVal != 0)));

    //     if (from != to )
    //     {
    //         map[f.x][f.y] = 0;

    //         moves.push_back(std::make_pair(std::make_pair(from, to), srcVal));

    //         animState = true;
    //         animClock.restart();
    //     }
    
    // }
}
