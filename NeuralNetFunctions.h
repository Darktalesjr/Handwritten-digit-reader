#include "includes.h"

#include "automata.h"

float actF(float x) {return(max(float(x / 10), x));}
float sactF(float x) {return(tanh(x));}//1 / (1 + exp(-x))

void evolve(automata* unit)
{
    automata* p1, * p2;
    for (int ct0 = FITTEST; ct0 < N; ct0++)
    {
        p1 = &unit[rand() % FITTEST];
        p2 = &unit[rand() % FITTEST];

        int ma = p1->neuralNet.actDim.size(), mi = p2->neuralNet.actDim.size();
        if (p2->neuralNet.actDim.size() > p1->neuralNet.actDim.size()) ma = p1->neuralNet.actDim.size(), mi = p2->neuralNet.actDim.size(), swap(p1, p2);
        unit[ct0].neuralNet.actDim.resize(ma);
        
    }
}

/*
if (bool(rand() % 2))
        {
            unit[ct0].neuralNet.actDim[0] = p1->neuralNet.actDim[0];
            unit[ct0].neuralNet.activation[0].resize(unit[ct0].neuralNet.actDim[0]);
            unit[ct0].neuralNet.weight[0].resize(unit[ct0].neuralNet.actDim[0]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[0]; ct1++)
            {
                for (int ct2 = 0; ct2 < IEXT; ct2++)
                {
                    if (bool(rand() % 2) && 0 < p1->neuralNet.activation[0].size())
                    {
                        unit[ct0].neuralNet.weight[0][ct1][ct2] = p2->neuralNet.weight[0][ct1][ct2];
                    }
                    else unit[ct0].neuralNet.weight[0][ct1][ct2] = p1->neuralNet.weight[0][ct1][ct2];
                }
                unit[ct0].neuralNet.bias[0][ct1] = p1->neuralNet.bias[0][ct1];
            }
        }
        else
        {
            unit[ct0].neuralNet.actDim[0] = p2->neuralNet.actDim[0];
            unit[ct0].neuralNet.activation[0].resize(unit[ct0].neuralNet.actDim[0]);
            unit[ct0].neuralNet.weight[0].resize(unit[ct0].neuralNet.actDim[0]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[0]; ct1++)
            {
                for (int ct2 = 0; ct2 < IEXT; ct2++)
                {
                    if (bool(rand() % 2) && 0 < p1->neuralNet.activation[0].size())
                    {
                        unit[ct0].neuralNet.weight[0][ct1][ct2] = p2->neuralNet.weight[0][ct1][ct2];
                        unit[ct0].neuralNet.bias[0][ct1] = p2->neuralNet.bias[0][ct1];
                    }
                    else unit[ct0].neuralNet.weight[0][ct1][ct2] = p1->neuralNet.weight[0][ct1][ct2];
                    unit[ct0].neuralNet.bias[0][ct1] = p1->neuralNet.bias[0][ct1];
                }
                unit[ct0].neuralNet.bias[0][ct1] = p2->neuralNet.bias[0][ct1];
            }
        }
        for (int ct = 1; ct < mi; ct++)
        {
            if (bool(rand() % 2)) unit[ct0].neuralNet.actDim[ct] = p1->neuralNet.actDim[ct];
            else unit[ct0].neuralNet.actDim[ct] = p2->neuralNet.actDim[ct];
            unit[ct0].neuralNet.activation[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            unit[ct0].neuralNet.weight[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[ct]; ct1++)
            {
                unit[ct0].neuralNet.weight[ct][ct1].resize(unit[ct0].neuralNet.actDim[ct]);
                for (int ct2 = 0; ct2 < unit[ct0].neuralNet.actDim[ct - 1]; ct2++)
                {
                    if (ct1 < p2->neuralNet.activation[ct].size() && ct2 < p2->neuralNet.weight[ct][ct1].size())
                    {
                        if (ct1 < p1->neuralNet.activation[ct].size() && ct2 < p1->neuralNet.weight[ct][ct1].size())
                        {
                            if (bool(rand() % 2)) { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p2->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                            else { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                        }
                        else { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p2->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                    }
                    else{ unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p1->neuralNet.bias[ct][ct1]; }
                }
            }
        }
        for (int ct = mi; ct < ma; ct++)
        {
            unit[ct0].neuralNet.actDim[ct] = p1->neuralNet.actDim[ct];
            unit[ct0].neuralNet.activation[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            unit[ct0].neuralNet.weight[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[ct]; ct1++)
            {
                unit[ct0].neuralNet.weight[ct][ct1].resize(unit[ct0].neuralNet.actDim[ct]);
                for (int ct2 = 0; ct2 < unit[ct0].neuralNet.actDim[ct - 1]; ct2++) { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p1->neuralNet.bias[ct][ct1];}
            }
        }
        */