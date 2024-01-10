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
    void train();
    void evaluate();
    inline void mutate();
    inline void swap(Automata* source);
};

void sort(Automata* source);