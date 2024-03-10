#include "NeuralNet.h"

void inline NeuralNet::initV()
{
    activation.resize(FACTDEPTH + 2);
    error.resize(FACTDEPTH + 1);
    zValue.resize(FACTDEPTH + 1);
    bias.resize(FACTDEPTH + 1);
    weight.resize(FACTDEPTH + 1);
    activation[0].resize(IEXT);
    for (int ct = 0; ct < FACTDEPTH; ct++)
    {
        activation[ct + 1].resize(FACTDIM);
        error[ct].resize(FACTDIM);
        zValue[ct].resize(FACTDIM);
        bias[ct].resize(FACTDIM);
        weight[ct].resize(FACTDIM);
        if (ct == 0) for (int ct1 = 0; ct1 < FACTDIM; ct1++) weight[0][ct1].resize(IEXT);
        else for (int ct1 = 0; ct1 < FACTDIM; ct1++) weight[ct][ct1].resize(FACTDIM);
    }
    error.back().resize(OEXT);
    activation.back().resize(OEXT);
    bias.back().resize(OEXT);
    zValue.back().resize(OEXT);
    weight.back().resize(OEXT);
    for (int ct1 = 0; ct1 < OEXT; ct1++) weight.back()[ct1].resize(FACTDIM);
}

void inline NeuralNet::initNNet()
{
    for (int ct = 0; ct < activation.size(); ct++) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) activation[ct][ct1] = 0;
    for (int ct = 0; ct < bias.size(); ct++) for (int ct1 = 0; ct1 < bias[ct].size(); ct1++) bias[ct][ct1] = 0;
    for (int ct = 0; ct < weight.size(); ct++) for (int ct1 = 0; ct1 < weight[ct].size(); ct1++) for (int ct2 = 0; ct2 < weight[ct][ct1].size(); ct2++) weight[ct][ct1][ct2] = genWeight();
}

void NeuralNet::copyNet(NeuralNet source)
{
    weight = source.weight;
    bias = source.bias;
}

NeuralNet::NeuralNet()
{
    initV();
    initNNet();
}

void NeuralNet::think()
{
    for (int ct = 1; ct < activation.size() - 1; ct++)
    {
        for (int ct1 = 0; ct1 < activation[ct].size(); ct1++)
        {
            zValue[ct - 1][ct1] = 0;
            for (int ct2 = 0; ct2 < activation[ct - 1].size(); ct2++) {
                zValue[ct - 1][ct1] += activation[ct - 1][ct2] * weight[ct - 1][ct1][ct2];
            }
            activation[ct][ct1] = actF(zValue[ct - 1][ct1]) + bias[ct - 1][ct1];
        }
    }
    for (int ct = 0; ct < OEXT; ct++)
    {
        zValue.back()[ct] = 0;
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++) zValue.back()[ct] += activation[activation.size() - 2][ct1] * weight.back()[ct][ct1];
        activation.back()[ct] = sactF(zValue.back()[ct]) + bias.back()[ct];
    }
    int ctt = 0;
    for (int ct = 0; ct < OEXT; ct++) if (activation.back()[ct] > activation.back()[ctt]) ctt = ct;
    cout << " - " << ctt << endl;
}

void NeuralNet::train()
{
    ifstream image_file("Resources/MNIST_hand-written_digits_dataset/train-images.idx3-ubyte", ios::in | ios::binary);
    ifstream label_file("Resources/MNIST_hand-written_digits_dataset/train-labels.idx1-ubyte", ios::in | ios::binary);
    char l, * pixels = new char[ROWS * COLS];

    label_file.seekg(8);
    image_file.seekg(16);
    for (int item_id = 0; item_id < 60000; ++item_id) {
        image_file.read(pixels, ROWS * COLS);
        label_file.read(&l, 1);
        for (int ct = 0; ct < OEXT; ct++) goal[ct] = 0;
        goal[l] = 1;
        cout << (int)l;
        for (int ct = 0; ct < COLS; ct++)for (int ct1 = 0; ct1 < ROWS; ct1++) activation[0][ROWS * ct1 + ct] = pixels[ROWS * ct1 + ct] / 255.0f;
        think();
        evaluate();

        if (item_id > 5990) SDL_Delay(1000);

    }
    delete[] pixels;
}


void NeuralNet::evaluate()
{
    for (int ct = 0; ct < activation.back().size(); ct++)
    {
        error.back()[ct] = 2 * (goal[ct] - activation.back()[ct]);
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++) error[error.size() - 2][ct1] -= weight.back()[ct][ct1] * sactFD(zValue[zValue.size() - 2][ct1]) * error.back()[ct];
    }

    for (int ct = activation.size() - 3; ct > 0; ct--) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) for (int ct2 = 0; ct2 < activation[ct + 1].size(); ct2++)
    {
        error[ct][ct1] += weight[ct][ct2][ct1] * sactFD(zValue[ct][ct1]) * error[ct][ct2];
    }

    for (int ct = activation.size() - 1; ct > 0; ct--) for (int ct1 = 0; ct1 < activation[ct].size(); ct1++) for (int ct2 = 0; ct2 < activation[ct - 1].size(); ct2++)
    {
        weight[ct - 1][ct1][ct2] -= sactFD(zValue[ct - 1][ct1]) * error[ct - 1][ct1];
    }
}