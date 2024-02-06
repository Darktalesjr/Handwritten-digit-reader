#pragma once
#include "NeuralNet.h"

inline void drawCircle(SDL_Renderer* renderer, int xc, int yc, int r);

inline void drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);

void renderDrawNet(SDL_Renderer* renderer, NeuralNet* net, TTF_Font* font);

void saveFile(NeuralNet* source, fstream* file); // WIP