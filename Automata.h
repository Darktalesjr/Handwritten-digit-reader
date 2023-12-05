#pragma once
#include "includes.h"

const float ACTIVATION_THRESHOLD = 0.5;
const int WIDTH = 640, HEIGHT = 480;
const int N = 2000, FITTEST = 9;
const int EXT = 20;
const int IEXT = 1, OEXT = 3;
const int MUTRATE_NEWT = 1, MUTRATE_RET = 1;
const Uint8 FACTDIM = 1, FACTDEPTH = 1;
const int GENTIMEMAX = 500;

const int WID = WIDTH / 2, HEI = HEIGHT / 2;
const int TXE = EXT / 2;
const int LIMIT = HEIGHT * 0.667;
const int L = LIMIT - TXE;
const int MUTRATE_NEW = 200 - MUTRATE_NEWT, MUTRATE_RE = 200 - MUTRATE_RET;

const int GOAL = 200;

#include "Player.h"
#include "NeuralNet.h"


struct Automata
{
    Player player;
    NeuralNet neuralNet;

    inline void think();
    inline void act();
    inline void mutate();
    inline void swap(Automata* source);
};

inline void sort(Automata* source) {
    for (int ct = 0; ct < N; ct++)source[ct].neuralNet.fitness = abs(20 - (source[ct].player.y < L) * 20);
    for (int ct = 1; ct < N;)
    {
        if (source[ct].neuralNet.fitness < source[ct - 1].neuralNet.fitness) {
            source[ct].swap(&source[ct - 1]);
            if (ct > 1) ct--;
        }
        else ct++;
    }
}