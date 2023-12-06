#pragma once
#include "includes.h"

float inline lreLU(float x) { return(max(float(x / 10.0f), x)); }
float inline reLU(float x) { return(max(0.0f, x)); }
float inline sigmoid(float x) { return (1 / (1 + exp(-x))); }

float inline actF(float x) { return(reLU(x)); }
float inline sactF(float x) { return(tanh(x)); }

float inline genWeight() { return ((((float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018))) / 1018) / 2.5 - 1); }

template<typename t> int inline max(vector<t> val); //Returns the largest element of a vector

void inline drawCircle(SDL_Renderer* renderer, int xc, int yc, int r);

void inline drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);

void inline renderDrawNet(SDL_Renderer* renderer, Automata* unit);

//void inline evolve(Automata* unit);