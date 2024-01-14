#pragma once
#include "includes.h"

struct NeuralNet
{
    vector<float> input;
    vector<float> output;
    vector<vector<float>> activation;
    vector<vector<float>> bias;
    vector<vector<vector<float>>> weight;
    vector<int> actDim;
    float fitness;

    void inline initV();
    void inline allocV();
    void inline initNNet();
    void think();
    void train();
    void evaluate();
    void copyNet(NeuralNet source);
    NeuralNet();
};