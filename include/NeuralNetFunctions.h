#pragma once
#include "includes.h"
#include "NeuralNet.h"

template<typename t> int inline max(vector<t> val); //Returns the largest element of a vector

inline void drawCircle(SDL_Renderer* renderer, int xc, int yc, int r);

inline void drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);

void renderDrawNet(SDL_Renderer* renderer, NeuralNet* unit, TTF_Font* font);