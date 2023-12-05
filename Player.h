#pragma once
#include "includes.h"

struct Player
{
    float x, y;
    float ax, ay;

    void initPlayer() { x = 320, y = L, ax = 0, ay = 0; }
    Player() { initPlayer(); }
};