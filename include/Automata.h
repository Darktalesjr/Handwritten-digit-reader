#pragma once
#include "includes.h"
#include "Player.h"
#include "NeuralNet.h"

struct Automata
{
    Player player;
    NeuralNet neuralNet;

    void think();
    void act();
    inline void mutate();
    inline void swap(Automata* source);
};

void sort(Automata* source);