#include "Automata.h"

void Automata::think()
{
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

void Automata::train()
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
        for (int ct = 0; ct < cols; ct++)for (int ct1 = 0; ct1 < rows; ct1++) neuralNet.input[rows * (ct1)+ct] = (Uint8)(pixels[rows * (ct1)+ct]) / 255.0F;
        think();
        act();
        evaluate();
    }
    delete[] pixels;
}

void Automata::act()
{
    if (neuralNet.output[0] > ACTIVATION_THRESHOLD) if (player.y == L)player.ay = 11;
    if (neuralNet.output[1] > ACTIVATION_THRESHOLD) player.x -= 1;
    if (neuralNet.output[2] > ACTIVATION_THRESHOLD) player.x += 1;
}

inline void Automata::mutate()
{
    for (int ct = 0; ct < neuralNet.activation.size(); ct++)
    {
        if ((rand() % (1018 / MUTRATE_NEW)) == 0)neuralNet.actDim[ct]++;
        else if (((rand() % (10180 / MUTRATE_NEW)) == 0) && neuralNet.actDim[ct] > 3)neuralNet.actDim[ct]--;
    }
}

inline void Automata::swap(Automata* source)
{
    neuralNet.weight.swap(source->neuralNet.weight);
    neuralNet.bias.swap(source->neuralNet.bias);
    std::swap(neuralNet.fitness, source->neuralNet.fitness);
}

void Automata::evaluate()
{
    float seed = 0;
    for (int ct = 0; ct < neuralNet.activation.back().size(); ct++)
    {
        neuralNet.activation.back()[ct] = 0;
        for (int ct1 = 0; ct1 < OEXT; ct1++) neuralNet.activation.back()[ct] += neuralNet.output[ct1] * neuralNet.weight.back()[ct][ct1];
    }
    for (int ct = neuralNet.activation.size(); ct > 1; ct--)
    {
        for (int ct1 = neuralNet.activation[ct].size(); ct1 > 0; ct1--)
        {
            seed = 0;
            for (int ct2 = 0; ct2 < neuralNet.activation[ct - 1].size(); ct2++) seed += neuralNet.activation[ct - 1][ct1] * neuralNet.weight[ct][ct2][ct1];
            neuralNet.activation[ct][ct1] = seed;
        }
    }
    for (int ct = 0; ct < neuralNet.activation[0].size(); ct++)
    {
        seed = 0;
        for (int ct1 = 0; ct1 < IEXT; ct1++) seed += neuralNet.input[ct1] * neuralNet.weight[0][ct][ct1];
        neuralNet.activation[0][ct] = seed;
    }

}

void sort(Automata* source) {
    for (int ct = 0; ct < N; ct++) source[ct].neuralNet.fitness = abs((source[ct].player.y < L) * 20) + abs(GOAL - source[ct].player.x);
    for (int ct = 1; ct < N;)
    {
        if (source[ct].neuralNet.fitness < source[ct - 1].neuralNet.fitness) {
            source[ct].swap(&source[ct - 1]);
            if (ct > 1) ct--;
        }
        else ct++;
    }
}
