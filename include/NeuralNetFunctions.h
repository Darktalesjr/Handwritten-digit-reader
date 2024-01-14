#pragma once
#include "NeuralNet.h"

void inline drawCircle(SDL_Renderer* renderer, int xc, int yc, int r);

void inline drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);

void renderDrawNet(SDL_Renderer* renderer, NeuralNet* net, TTF_Font* font);

void saveFile(NeuralNet* source, fstream* file); // WIP