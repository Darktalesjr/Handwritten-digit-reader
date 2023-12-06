#pragma once
#include "includes.h"

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