#pragma once

const int N = 4, M = 4,
    TILESIZE = 64,
    WIDTH = N*TILESIZE,
    HEIGHT = M*TILESIZE+TILESIZE;

enum class Direction
{
    Left,
    Right,
    Up,
    Down,
};