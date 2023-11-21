#pragma once
#include "includes.h"

const float ACTIVATION_THRESHOLD = 0.5;
const int WIDTH = 640, HEIGHT = 640;
const int N = 63, FITTEST = 9;
const int EXT = 20;
const int TXE = EXT / 2;
const int NEXT = 40, NLENGHT = 30, IEXT = 4, OEXT = 3;
const int LIMIT = HEIGHT * 0.667;
const int L = LIMIT - TXE;
const int GOAL[2]{ WIDTH - TXE, 306 };
const int MUTRATE = 1;
const Uint8 FACTDIM = 10;
const Uint8 FACTDEPTH = 10;

float actF(float x);
float sactF(float x);

struct player
{
    float x, y;
    float ax, ay;
    bool j;

    void initPlayer() { x = 320, y = L, ax = 0, ay = 0; }
    player() { initPlayer(); }
};

struct nNet
{
    vector<float> input;
    vector<float> output;
    vector<vector<float>> activation;
    vector<vector<float>> bias;
    vector<vector<vector<float>>> weight;
    vector<Uint8> actDim;
    float fitness;

    void initV()
    {
        input.resize(IEXT);
        output.resize(OEXT);

        activation.resize(FACTDEPTH);
        actDim.resize(FACTDEPTH);
        bias.resize(FACTDEPTH + 1);
        weight.resize(FACTDEPTH + 1);
        for (int ct = 0; ct < FACTDEPTH; ct++)
        {
            activation[ct].resize(FACTDIM);
            bias[ct].resize(FACTDIM);
            weight[ct].resize(FACTDIM);
            actDim[ct] = FACTDIM;
            if (ct == 0)
            {
                for (int ct1 = 0; ct1 < FACTDIM; ct1++)
                {
                    weight[0][ct1].resize(IEXT);
                }
            }
            else
            {
                for (int ct1 = 0; ct1 < FACTDIM; ct1++)
                {
                    weight[ct][ct1].resize(FACTDIM);
                }
            }
        }
        weight.back().resize(OEXT);
        for (int ct1 = 0; ct1 < OEXT; ct1++)
        {
            weight.back()[ct1].resize(FACTDIM);
        }
        bias.back().resize(OEXT);
    }

    void allocV()
    {

        int lgt = actDim.size();
        activation.resize(lgt);
        bias.resize(lgt);
        weight.resize(lgt);

        for (int ct = 0; ct < lgt; ct++)
        {
            activation[ct].resize(actDim[ct]);
            bias[ct].resize(actDim[ct]);
            weight[ct].resize(actDim[ct]);
            if (ct != 0)
            {
                for (int ct1 = 0; ct1 < actDim[ct]; ct1++)
                {
                    weight[ct][ct1].resize(actDim[ct]);
                }
            }
        }
        bias.back().resize(OEXT);
    }

    void initNNet()
    {
        for (int ct = 0; ct < activation.size(); ct++) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) activation[ct][ct1] = 0;
        for (int ct = 0; ct < bias.size(); ct++) for (int ct1 = 0; ct1 < bias[ct].size(); ct1++) bias[ct][ct1] = 0;
        for (int ct = 0; ct < weight[0].size(); ct++) for (int ct1 = 0; ct1 < weight[0][ct].size(); ct1++) weight[0][ct][ct1] = (((float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 2036))) / 1018) - 1;
        for (int ct = 1; ct < weight.size(); ct++) for (int ct1 = 0; ct1 < weight[ct].size(); ct1++) for (int ct2 = 0; ct2 < weight[ct][ct1].size(); ct2++) weight[ct][ct1][ct2] = ((((float(rand() % 1018)) + float(rand() % 1018))) / 1018) - 1;
    }

    void copyNet(nNet source)
    {
        activation = source.activation;
        weight = source.weight;
        bias = source.bias;
        actDim = source.actDim;
    }
};


struct automata
{
    player player;
    nNet neuralNet;

    void think()
    {
        neuralNet.input[0] = player.x;
        neuralNet.input[1] = player.y;
        neuralNet.input[2] = GOAL[0] - player.x;
        neuralNet.input[3] = GOAL[1] - player.y;

        float seed = 0;

        for (int ct = 0; ct < neuralNet.activation[0].size(); ct++)
        {
            seed = 0;
            for (int ct1 = 0; ct1 < IEXT; ct1++) seed += neuralNet.input[ct1] * neuralNet.weight[0][ct][ct1];
            neuralNet.activation[0][ct] = actF(seed);
        }
        for (int ct = 1; ct < neuralNet.activation.size(); ct++)
        {
            for (int ct1 = 0; ct1 < neuralNet.activation[ct].size(); ct1++)
            {
                seed = 0;
                for (int ct2 = 0; ct2 < neuralNet.activation[ct - 1].size(); ct2++) seed += neuralNet.activation[ct - 1][ct1] * neuralNet.weight[ct][ct2][ct1];
                neuralNet.activation[ct][ct1] = actF(seed + neuralNet.bias[ct][ct1]);
            }
        }
        for (int ct = 0; ct < OEXT; ct++)
        {
            seed = 0;
            for (int ct1 = 0; ct1 < neuralNet.activation.back().size(); ct1++) seed += neuralNet.activation.back()[ct1] * neuralNet.weight.back()[ct][ct1];
            neuralNet.output[ct] = sactF(seed + neuralNet.bias.back()[ct]);
        }

    }
    void inline act()
    {
        if (neuralNet.output[0] > ACTIVATION_THRESHOLD) if (player.y == L)player.ay = 11;
        if (neuralNet.output[1] > ACTIVATION_THRESHOLD) player.x -= 1;
        if (neuralNet.output[2] > ACTIVATION_THRESHOLD) player.x += 1;
    }

    void mutate()
    {
        for (int ct = 0; ct < neuralNet.activation.size(); ct++)
        {
            if ((rand() % (1018 / MUTRATE)) == 0)neuralNet.actDim[ct]++;
            else if (((rand() % (10180 / MUTRATE)) == 0) && neuralNet.actDim[ct] > 3)neuralNet.actDim[ct]--;
        }
    }

    automata()
    {
        neuralNet.initV();
        neuralNet.initNNet();
    }
};

inline void swap(automata* automata1,  automata* automata2)
{
    automata1->neuralNet.weight.swap(automata2->neuralNet.weight);
    automata1->neuralNet.bias.swap(automata2->neuralNet.bias);
    swap(automata1->neuralNet.fitness, automata2->neuralNet.fitness);
}
/*
Uint8 n = 0;
        double m = neuralNet.output[0];
        for (int ct = 1; ct < OEXT; ct++) {
            if (neuralNet.output[ct] > m) n = ct;
        }
        switch (n)
        {
        case 0:
            if (player.y == L)player.ay = 11;
            break;
        case 1:
            player.x -= 1;
            break;
        case 2:
            player.x += 1;
            break;
        }
*/

/*
void player::initPlayer() { x = 320, y = L, ax = 0, ay = 0; }


void nNet::initV()
{
    input.resize(IEXT);
    output.resize(OEXT);

    activation.resize(FACTDEPTH);
    actDim.resize(FACTDEPTH);
    bias.resize(FACTDEPTH + 1);
    weight.resize(FACTDEPTH + 1);
    for (int ct = 0; ct < FACTDEPTH; ct++)
    {
        activation[ct].resize(FACTDIM);
        bias[ct].resize(FACTDIM);
        weight[ct].resize(FACTDIM);
        actDim[ct] = FACTDIM;
        if (ct == 0)
        {
            for (int ct1 = 0; ct1 < FACTDIM; ct1++)
            {
                weight[0][ct1].resize(IEXT);
            }
        }
        else
        {
            for (int ct1 = 0; ct1 < FACTDIM; ct1++)
            {
                weight[ct][ct1].resize(FACTDIM);
            }
        }
    }
    weight.back().resize(OEXT);
    for (int ct1 = 0; ct1 < OEXT; ct1++)
    {
        weight.back()[ct1].resize(FACTDIM);
    }
    bias.back().resize(OEXT);
}

void nNet::allocV()
{

    int lgt = actDim.size();
    activation.resize(lgt);
    bias.resize(lgt);
    weight.resize(lgt);

    for (int ct = 0; ct < lgt; ct++)
    {
        activation[ct].resize(actDim[ct]);
        bias[ct].resize(actDim[ct]);
        weight[ct].resize(actDim[ct]);
        if (ct != 0)
        {
            for (int ct1 = 0; ct1 < actDim[ct]; ct1++)
            {
                weight[ct][ct1].resize(actDim[ct]);
            }
        }
    }
    bias.back().resize(OEXT);
}

void nNet::initNNet()
{
    for (int ct = 0; ct < activation.size(); ct++) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) activation[ct][ct1] = 0;
    for (int ct = 0; ct < bias.size(); ct++) for (int ct1 = 0; ct1 < bias[ct].size(); ct1++) bias[ct][ct1] = 0;
    for (int ct = 0; ct < weight[0].size(); ct++) for (int ct1 = 0; ct1 < weight[0][ct].size(); ct1++) weight[0][ct][ct1] = ((((float(rand() % 1018)) + float(rand() % 1018))) / 1018) - 1;
    for (int ct = 1; ct < weight.size(); ct++) for (int ct1 = 0; ct1 < weight[ct].size(); ct1++) for (int ct2 = 0; ct2 < weight[ct][ct1].size(); ct2++) weight[ct][ct1][ct2] = ((((float(rand() % 1018)) + float(rand() % 1018))) / 1018) - 1;
}

void nNet::copyNet(nNet source)
{
    activation = source.activation;
    weight = source.weight;
    bias = source.bias;
    actDim = source.actDim;
}

void automata::think()
{
    neuralNet.input[0] = player.x;
    neuralNet.input[1] = player.y;
    neuralNet.input[2] = GOAL[0] - player.x;
    neuralNet.input[3] = GOAL[1] - player.y;

    float seed = 0;

    for (int ct = 0; ct < neuralNet.activation[0].size(); ct++)
    {
        seed = 0;
        for (int ct1 = 0; ct1 < IEXT; ct1++) seed += neuralNet.input[ct1] * neuralNet.weight[0][ct][ct1];
        neuralNet.activation[0][ct] = actF(seed);
    }
    for (int ct = 1; ct < neuralNet.activation.size(); ct++)
    {
        for (int ct1 = 0; ct1 < neuralNet.activation[ct].size(); ct1++)
        {
            seed = 0;
            for (int ct2 = 0; ct2 < neuralNet.activation[ct - 1].size(); ct2++) seed += neuralNet.activation[ct - 1][ct1] * neuralNet.weight[ct][ct2][ct1];
            neuralNet.activation[ct][ct1] = actF(seed + neuralNet.bias[ct][ct1]);
        }
    }
    for (int ct = 0; ct < OEXT; ct++)
    {
        seed = 0;
        for (int ct1 = 0; ct1 < neuralNet.activation.back().size(); ct1++) seed += neuralNet.activation.back()[ct1] * neuralNet.weight.back()[ct][ct1];
        neuralNet.output[ct] = sactF(seed + neuralNet.bias.back()[ct]);
    }
}

void automata::act()
{
    Uint8 n = 0;
    double m = neuralNet.output[0];
    for (int ct = 1; ct < OEXT; ct++) {
        if (neuralNet.output[ct] > m) n = ct;
    }
    switch (n)
    {
    case 0:
        if (player.y == L)player.ay = 11;
        break;
    case 1:
        player.x -= 1;
        break;
    case 2:
        player.x += 1;
        break;
    }

}

void automata::mutate()
{
    for (int ct = 0; ct < neuralNet.activation.size(); ct++)
    {
        if ((rand() % (1018 / MUTRATE)) == 0)neuralNet.actDim[ct]++;
        else if (((rand() % (10180 / MUTRATE)) == 0) && neuralNet.actDim[ct] > 3)neuralNet.actDim[ct]--;
    }
}

automata::automata()
{
    neuralNet.initV();
    player.initPlayer();
    neuralNet.initNNet();
}*/