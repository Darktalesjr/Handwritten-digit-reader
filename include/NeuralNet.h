#pragma once
#include "includes.h"

struct NeuralNet
{
    float goal[OEXT];

    vector<vector<float>> activation;
    vector<vector<float>> zValue;
    vector<vector<float>> error;
    vector<vector<float>> bias;
    vector<vector<vector<float>>> weight;

    void think();
    void train();
    void evaluate();
    void inline initV();
    void inline initNNet();
    void copyNet(NeuralNet source);
    NeuralNet();
};