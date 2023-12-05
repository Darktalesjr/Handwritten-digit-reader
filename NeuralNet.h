#pragma once
#include "Includes.h"
struct NeuralNet
{
    vector<float> input;
    vector<float> output;
    vector<vector<float>> activation;
    vector<vector<float>> bias;
    vector<vector<vector<float>>> weight;
    vector<Uint8> actDim;
    float fitness;

    void inline initV();
    void inline allocV();
    void inline initNNet();
    void copyNet(NeuralNet source);
    NeuralNet();
};