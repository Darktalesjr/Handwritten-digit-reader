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

void NeuralNet::think()
{
    float seed = 0;

    for (int ct = 0; ct < activation[0].size(); ct++)
    {
        seed = 0;
        for (int ct1 = 0; ct1 < IEXT; ct1++) seed += input[ct1] * weight[0][ct][ct1];
        activation[0][ct] = actF(seed);
    }
    for (int ct = 1; ct < activation.size(); ct++)
    {
        for (int ct1 = 0; ct1 < activation[ct].size(); ct1++)
        {
            seed = 0;
            for (int ct2 = 0; ct2 < activation[ct - 1].size(); ct2++) seed += activation[ct - 1][ct1] * weight[ct][ct2][ct1];
            activation[ct][ct1] = actF(seed + bias[ct][ct1]);
        }
    }
    for (int ct = 0; ct < OEXT; ct++)
    {
        seed = 0;
        for (int ct1 = 0; ct1 < activation.back().size(); ct1++) seed += activation.back()[ct1] * weight.back()[ct][ct1];
        output[ct] = sactF(seed + bias.back()[ct]);
    }

}
void NeuralNet::train()
{
    ifstream image_file("Resources/Datasets/MNIST_hand-written_digits_dataset/train-images.idx3-ubyte", ios::in | ios::binary);
    ifstream label_file("Resources/Datasets/MNIST_hand-written_digits_dataset/train-labels.idx1-ubyte", ios::in | ios::binary);
    char l, * pixels = new char[rows * cols];

    label_file.seekg(8);
    image_file.seekg(16);

    for (int item_id = 0; item_id < 60000; ++item_id) {
        image_file.read(pixels, rows * cols);
        label_file.read(&l, 1);
        uint8_t label = (int)l;
        for (int ct = 0; ct < cols; ct++)for (int ct1 = 0; ct1 < rows; ct1++) input[rows * (ct1)+ct] = (Uint8)(pixels[rows * (ct1)+ct]) / 255.0F;
        think();
        evaluate();
    }
    delete[] pixels;
}

void NeuralNet::evaluate()
{
    float seed = 0;
    for (int ct = 0; ct < activation.back().size(); ct++)
    {
        activation.back()[ct] = 0;
        for (int ct1 = 0; ct1 < OEXT; ct1++)
        {
            activation.back()[ct] += output[ct1] * weight.back()[ct1][ct];
            //weight.back()[ct1][ct] += weight.back()[ct1][ct] * output[ct1];
        }
    }

    for (int ct = activation.size()-1; ct > 1; ct--) for (int ct1 = activation[ct].size()-1; ct1 > 0; ct1--)
    {
        activation[ct][ct1] = 0;
        for (int ct2 = 0; ct2 < activation[ct - 1].size(); ct2++) activation[ct][ct1] += activation[ct - 1][ct1] * weight[ct][ct2][ct1];
    }

    for (int ct = 0; ct < activation[0].size(); ct++)
    {
        activation[0][ct] = 0;
        for (int ct1 = 0; ct1 < IEXT; ct1++) activation[0][ct] += input[ct1] * weight[0][ct][ct1];
    }

}