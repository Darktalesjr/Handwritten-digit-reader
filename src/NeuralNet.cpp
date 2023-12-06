#include "NeuralNet.h"

void inline NeuralNet::initV()
{
    input.resize(IEXT);
    output.resize(OEXT);

    activation.resize(FACTDEPTH);
    actDim.resize(FACTDEPTH);
    bias.resize(FACTDEPTH + 1);
    weight.resize(FACTDEPTH + 1);
    for (int ct = 0; ct < FACTDEPTH; ct++) {
        activation[ct].resize(FACTDIM);
        bias[ct].resize(FACTDIM);
        weight[ct].resize(FACTDIM);
        actDim[ct] = FACTDIM;
        if (ct == 0) for (int ct1 = 0; ct1 < FACTDIM; ct1++) weight[0][ct1].resize(IEXT);
        else for (int ct1 = 0; ct1 < FACTDIM; ct1++) weight[ct][ct1].resize(FACTDIM);
    }
    weight.back().resize(OEXT);
    for (int ct1 = 0; ct1 < OEXT; ct1++) weight.back()[ct1].resize(FACTDIM);
    bias.back().resize(OEXT);
}

void inline NeuralNet::allocV()
{
    int lgt = actDim.size();
    activation.resize(lgt);
    bias.resize(lgt + 1);
    weight.resize(lgt + 1);
    for (int ct = 0; ct < lgt; ct++)
    {
        activation[ct].resize(actDim[ct]);
        bias[ct].resize(actDim[ct]);
        weight[ct].resize(actDim[ct]);
        if (ct != 0) for (int ct1 = 0; ct1 < actDim[ct]; ct1++) weight[ct][ct1].resize(actDim[ct]);
    }
    bias.back().resize(OEXT);
}

void inline NeuralNet::initNNet()
{
    for (int ct = 0; ct < activation.size(); ct++) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) activation[ct][ct1] = 0;
    for (int ct = 0; ct < bias.size(); ct++) for (int ct1 = 0; ct1 < bias[ct].size(); ct1++) bias[ct][ct1] = 0;
    for (int ct = 0; ct < weight[0].size(); ct++) for (int ct1 = 0; ct1 < weight[0][ct].size(); ct1++) weight[0][ct][ct1] = genWeight();
    for (int ct = 1; ct < weight.size(); ct++) for (int ct1 = 0; ct1 < weight[ct].size(); ct1++) for (int ct2 = 0; ct2 < weight[ct][ct1].size(); ct2++) weight[ct][ct1][ct2] = genWeight();
}

void NeuralNet::copyNet(NeuralNet source)
{
    activation = source.activation;
    weight = source.weight;
    bias = source.bias;
    actDim = source.actDim;
}

NeuralNet::NeuralNet()
{
    initV();
    initNNet();
}