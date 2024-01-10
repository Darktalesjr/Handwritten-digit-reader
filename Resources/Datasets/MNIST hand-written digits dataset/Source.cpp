#include <iostream>
#include <fstream>
#include "SDL.h"
#include <string>

using namespace std;



void read_mnist_cv(SDL_Renderer* renderer, const char* image_filename, const char* label_filename) {
    ifstream image_file(image_filename, ios::in | ios::binary);
    ifstream label_file(label_filename, ios::in | ios::binary);

    uint32_t magic;
    uint32_t rows = 28;
    uint32_t cols = 28;

    label_file.seekg(8);
    image_file.seekg(16);

    char label;
    char* pixels = new char[rows * cols];
    SDL_Rect dr{0,0,20,20};
    Uint8 c;

    for (int item_id = 0; item_id < 60000; ++item_id) {
        image_file.read(pixels, rows * cols);
        label_file.read(&label, 1);
        cout << int(label) << endl;

        for (int ct = 0; ct < cols; ct++)for (int ct1 = 0; ct1 < rows; ct1++)
        {
            c = (Uint8)(pixels[rows*(ct1)+ct]);
            SDL_SetRenderDrawColor(renderer, c, c, c, 255);
            dr.x = ct * 20; dr.y = ct1 * 20;
            SDL_RenderFillRect(renderer, &dr);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    delete[] pixels;
}

int main(int argc, char* argv[])
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int width = 560, height = 560;

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Temp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Event event;

    bool exit = true;
    while (exit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                exit = false;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    exit = false;
                    break;
                }
            }
        }
        read_mnist_cv(renderer,"C:/Users/nicol/source/repos/Temp/Resources/MNIST_hand-written_digits_dataset/train-images.idx3-ubyte", "C:/Users/nicol/source/repos/Temp/Resources/MNIST_hand-written_digits_dataset/train-labels.idx1-ubyte");
    }
    return 0;
}

/*
uint32_t swap_endian(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}
*/