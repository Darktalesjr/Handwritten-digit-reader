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
        if (ct == 0)
            for (int ct1 = 0; ct1 < FACTDIM; ct1++)
                weight[0][ct1].resize(IEXT);
        else
            for (int ct1 = 0; ct1 < FACTDIM; ct1++)
                weight[ct][ct1].resize(FACTDIM);
    }
    error.back().resize(OEXT);
    activation.back().resize(OEXT);
    bias.back().resize(OEXT);
    zValue.back().resize(OEXT);
    weight.back().resize(OEXT);
    cost.resize(OEXT);
    for (int ct1 = 0; ct1 < OEXT; ct1++)
        weight.back()[ct1].resize(FACTDIM);
}

void inline NeuralNet::initNNet()
{
    for (int ct = 0; ct < activation.size(); ct++)
        for (int ct1 = 0; ct1 < activation[ct].size(); ct1++)
            activation[ct][ct1] = 0;
    for (int ct = 0; ct < bias.size(); ct++)
        for (int ct1 = 0; ct1 < bias[ct].size(); ct1++)
            bias[ct][ct1] = 0;
    for (int ct = 0; ct < weight.size(); ct++)
        for (int ct1 = 0; ct1 < weight[ct].size(); ct1++)
            for (int ct2 = 0; ct2 < weight[ct][ct1].size(); ct2++)
                weight[ct][ct1][ct2] = (ct != weight.size() - 1) ? genWeight((ct == 0) ? IEXT : FACTDIM, FACTDIM) : genWeightO(FACTDIM, OEXT);
    for (int ct = 0; ct < OEXT; ct++) goal[ct] = 0;
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
            activation[ct][ct1] = actF(zValue[ct - 1][ct1] + bias[ct - 1][ct1]);
        }
    }
    for (int ct = 0; ct < OEXT; ct++)
    {
        zValue.back()[ct] = 0;
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++) zValue.back()[ct] += activation[activation.size() - 2][ct1] * weight.back()[ct][ct1];
        activation.back()[ct] = sactF(zValue.back()[ct] + bias.back()[ct]);
    }
    int ctt = 0;
    for (int ct = 0; ct < OEXT; ct++)if (activation.back()[ct] > activation.back()[ctt]) ctt = ct;
}

void NeuralNet::train(bool* running)
{
    printf("\x1b[?25l\x1b[2J");
    ifstream image_file("Resources/MNIST_hand-written_digits_dataset/train-images.idx3-ubyte", ios::in | ios::binary);
    ifstream label_file("Resources/MNIST_hand-written_digits_dataset/train-labels.idx1-ubyte", ios::in | ios::binary);
    char l = 0, * pixels = new char[ROWS * COLS];
    char progress[3];

    label_file.seekg(8);
    image_file.seekg(16);
    fputs(" [                                                  ]", stdout); 
    for (int item_id = 0; item_id < 60000; ++item_id) {
        image_file.read(pixels, ROWS * COLS);
        goal[l] = 0; label_file.read(&l, 1); goal[l] = 1;
        for (int ct = 0; ct < COLS; ct++)for (int ct1 = 0; ct1 < ROWS; ct1++) activation[0][COLS * ct + ct1] = pixels[COLS * ct + ct1] / 255.0f;
        think();
        evaluate();
        if (item_id % GROUP_SIZE == 0) descent();
        if (!*running) break;

        if (item_id % 600 == 0 && item_id != 0) 
        {
            fputs("\r\x1b[2C", stdout);
            for (int ct = 0; ct < (item_id) / 1200; ct++) fputs("=", stdout);
            snprintf(progress, 3, "%d", (item_id + 00) / 600);
            fputs("\r\x1b[24C[   %]\x1b[5D", stdout);
            if (item_id / 600 < 10) fputs(" ", stdout);
            if (item_id / 600 < 100) fputs(" ", stdout);
            fputs(progress, stdout);
            fputs("\r", stdout);
	    }
        item_id += 50;
    }
    for (int ct = 0; ct < activation.size(); ct++)
        for (int ct1 = 0; ct1 < activation[ct].size(); ct1++)
            activation[ct][ct1] = 0;
    delete[] pixels;
	isTrained = true;
    cout << "\n Trained!" << endl;
}

void NeuralNet::sandbox(bool* running)
{
    int wDiff = WIDTH / (activation.size() + 2);
    int hDiff = HEIGHT / max(FACTDIM, OEXT);
    int px = 0, py = 0;
    while (*running)
    {
        if (isTrained)
        {
            SDL_GetMouseState(&px, &py);
            if ((px > ((wDiff * 3 - ZOOM_ICON * ROWS) / 2 - 1)) && (px < ((wDiff * 3 - ZOOM_ICON * ROWS) / 2 - 1 + ZOOM_ICON * ROWS + 2))  && (py > (HEI - ZOOM_ICON * ROWS / 2 - 1)) && (py < (HEI - ZOOM_ICON * ROWS / 2 - 1 + ZOOM_ICON * COLS + 2)))
            {
                px -= (wDiff * 3 - ZOOM_ICON * ROWS) / 2 + 1;
                py -= HEI - ZOOM_ICON * ROWS / 2 + 1;
                px /= ZOOM_ICON;
                py /= ZOOM_ICON;
                switch (SDL_GetMouseState(NULL,NULL))
                {
                case 1:
                    activation[0][py * COLS + px] = 1;
                    think();
                    break;
                case 4:
                    activation[0][py * COLS + px] = 0;
                    think();
                    break;
                }
            }
        }
    }
}

void inline NeuralNet::evaluate()
{
    for (int ct = 0; ct < activation.back().size(); ct++)
        error.back()[ct] += 2 * (activation.back()[ct] - goal[ct]),
        cost[ct] += (activation.back()[ct] - goal[ct]) * (activation.back()[ct] - goal[ct]);
}

void inline NeuralNet::descent()
{
    for (int ct = 0; ct < error.back().size(); ct++)
        error.back()[ct] /= GROUP_SIZE;

    for (int ct = 0; ct < activation.back().size(); ct++)
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++)
            error[error.size() - 2][ct1] += weight.back()[ct][ct1] * sactFD(zValue.back()[ct]) * error.back()[ct];

    for (int ct0 = activation.size() - 2; ct0 > 1; ct0--)
        for (int ct = 0; ct < activation[ct0].size(); ct++)
            for (int ct1 = 0; ct1 < activation[ct0 - 1].size(); ct1++)
                error[ct0 - 2][ct1] += weight[ct0-1][ct][ct1] * actFD(zValue[ct0-1][ct]) * error[ct0-1][ct];


    for (int ct = 0; ct < activation.back().size(); ct++)
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++)
			weight.back()[ct][ct1] -= error.back()[ct] * activation[activation.size() - 2][ct1] * LEARNING_RATE;
    for (int ct0 = activation.size() - 2; ct0 > 1; ct0--)
        for (int ct = 0; ct < activation[ct0].size(); ct++)
            for (int ct1 = 0; ct1 < activation[ct0 - 1].size(); ct1++)
                weight[ct0 - 1][ct][ct1] -= activation[ct0-1][ct1] * error[ct0 - 1][ct] * LEARNING_RATE;
    for (int ct = 0; ct < activation[1].size(); ct++)
		for (int ct1 = 0; ct1 < activation[0].size(); ct1++)
			weight[0][ct][ct1] -= activation[0][ct1] * error[0][ct] * LEARNING_RATE;

    for (int ct = 0; ct < activation.back().size(); ct++)
        for (int ct1 = 0; ct1 < activation[activation.size() - 2].size(); ct1++)
            bias.back()[ct] -= error.back()[ct] * LEARNING_RATE;

    for (int ct0 = activation.size() - 2; ct0 > 1; ct0--)
        for (int ct = 0; ct < activation[ct0].size(); ct++)
            for (int ct1 = 0; ct1 < activation[ct0 - 1].size(); ct1++)
                bias[ct0 - 1][ct] -= error[ct0 - 1][ct] * LEARNING_RATE;
    for (int ct = 0; ct < activation[1].size(); ct++)
        for (int ct1 = 0; ct1 < activation[0].size(); ct1++)
            bias[0][ct] -= error[0][ct] * LEARNING_RATE;

    for (int ct = 0; ct < error.size(); ct++)
        for (int ct1 = 0; ct1 < error[ct].size(); ct1++)
            error[ct][ct1] = 0;
}