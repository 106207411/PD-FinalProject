#pragma once
#include "Settings.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

namespace mn
{
    class Menu;
}

namespace gm
{
    class Game
    {
    public:
        Game(int w, int h);
        void setMode(int mode);
        int getMode();
        void onEvent(sf::Event e, mn::Menu& menu);
        void update();
        void render(sf::RenderTarget& tgt);

        void spawn();
        void reset();
        void coutMap();

    private:
        int tileSize;
        int headerSize;
        int mode;         // 0 - 2048, 1 - school
        int gameRound;
        char map[FIELD_WIDTH][FIELD_HEIGHT] = { 0 };
        char tempMap[FIELD_WIDTH][FIELD_HEIGHT] = { 0 };
        sf::Clock animClock;
        sf::RectangleShape restartButton;
        sf::RectangleShape menuButton;
        sf::Text textScore;
        bool isHover_menu;
        bool isHover_restart;
        bool animState;
        bool chanceYes;
        bool destinyYes;
        bool isGameOver;
        bool isGameWon;

        sf::Font font;

        std::vector<std::pair<std::pair<sf::Vector2i, sf::Vector2i>, char>> moves;

        sf::Color getTileColor(char tile);
        sf::Color getTextColor(char tile);
        std::wstring getText(char tile);
        int getTextSize(char tile);
        void move(char dirX, char dirY);
        void tileMove(sf::Vector2i from, sf::Vector2i to, bool is_merge);
        void chanceAndDestiny(); //機會與命運
        void deletechanceAndDenstiny(); //清掉機會與命運
    };
}
