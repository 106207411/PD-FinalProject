#pragma once
#include "Settings.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

namespace gm
{
    class Game
    {
    public:
        Game(int w, int h);
        void setMode(int mode);
        int getMode();
        void OnEvent(sf::Event e);
        void Update();
        void Render(sf::RenderTarget& tgt);

        void Spawn();
        void Reset();

    private:
        int tileSize;
        int headerSize;
        int mode;         // 0 - 2048, 1 - school
        char map[FIELD_WIDTH][FIELD_HEIGHT] = { 0 };
        char tempMap[FIELD_WIDTH][FIELD_HEIGHT] = { 0 };
        sf::Clock animClock;
        sf::RectangleShape restartButton;
        bool animState;
        bool chanceYes;
        bool destinyYes;

        sf::Font font;

        std::vector<std::pair<std::pair<sf::Vector2i, sf::Vector2i>, char>> moves;

        sf::Color getTileColor(char tile);
        sf::Color getTextColor(char tile);
        std::wstring getText(char tile);
        int getTextSize(char tile);
        void move(char dirX, char dirY);
        void applyMove(sf::Vector2i from, sf::Vector2i to, int dx, int dy);
        
    };
}
