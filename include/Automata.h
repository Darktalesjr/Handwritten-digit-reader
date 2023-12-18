#pragma once
#include "includes.h"
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

inline void sort(Automata* source);