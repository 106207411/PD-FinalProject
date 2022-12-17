#pragma once

const int WIDTH_PIXEL = 4, HEIGHT_PIXEL = 4,
    TILESIZE = 64,
    WIDTH = WIDTH_PIXEL*TILESIZE,
    HEIGHT = HEIGHT_PIXEL*TILESIZE+TILESIZE;

enum class Direction
{
    Left,
    Right,
    Up,
    Down,
};