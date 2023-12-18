#pragma once
#pragma warning (disable : 4244 267)

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "SDL_ttf.h"
#include "SDL.h"

using namespace std;

const float ACTIVATION_THRESHOLD = 0.5;
const int WIDTH = 640, HEIGHT = 480;
const int N = 2000, FITTEST = 9;
const int EXT = 20;
const int IEXT = 1, OEXT = 3;
const int MUTRATE_NEWT = 1, MUTRATE_RET = 1;
const int FACTDIM = 1, FACTDEPTH = 1;
const int GENTIMEMAX = 500;

const int WID = WIDTH / 2, HEI = HEIGHT / 2;
const int TXE = EXT / 2;
const int LIMIT = HEIGHT * 0.667;
const int L = LIMIT - TXE;
const int MUTRATE_NEW = 200 - MUTRATE_NEWT, MUTRATE_RE = 200 - MUTRATE_RET;

const int GOAL = 200;


const SDL_Color WHITE = { 255, 255, 255, 255 };

float inline lreLU(float x) { return(max(float(x / 10.0f), x)); }
float inline reLU(float x) { return(max(0.0f, x)); }
float inline sigmoid(float x) { return (1 / (1 + exp(-x))); }

float inline actF(float x) { return(reLU(x)); }
float inline sactF(float x) { return(tanh(x)); }

float inline genWeight() { return ((((float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018))) / 1018) / 2.5 - 1); }