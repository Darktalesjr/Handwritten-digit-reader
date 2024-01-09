#pragma once
#include "includes.h"
#include "Automata.h"

template<typename t> int inline max(vector<t> val); //Returns the largest element of a vector

void inline drawCircle(SDL_Renderer* renderer, int xc, int yc, int r);

void inline drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);

void renderDrawNet(SDL_Renderer* renderer, Automata* unit, TTF_Font* font);

void evolve(Automata* unit);

void saveFile(Automata* source, fstream* file);