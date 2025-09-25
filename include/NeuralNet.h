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
	vector<float> cost;
	bool isTrained = false;

    void inline think();
    void train(bool *running);
    void inline evaluate();
    void inline descent();
    void inline initV();
    void inline initNNet();
    void sandbox(bool *running);
    NeuralNet();
};