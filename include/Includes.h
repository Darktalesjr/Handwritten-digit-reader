#pragma once
#pragma warning (disable : 4244 267)

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

#include "SDL_ttf.h"
#include "SDL.h"

using namespace std;

const float ACTIVATION_THRESHOLD = 0.5;
const int ZOOM = 20;

const int N = 2000, FITTEST = 9;
const int IEXT = 28 * 28, OEXT = 10;
const int MUTRATE_NEWT = 1, MUTRATE_RET = 1;
const int FACTDIM = 16, FACTDEPTH = 2;
const int GENTIMEMAX = 500;
const int GOAL = 200;

const uint32_t rows = 28;
const uint32_t cols = 28;

const int WIDTH = ZOOM * cols, HEIGHT = ZOOM * rows;
const int WID = WIDTH / 2, HEI = HEIGHT / 2;

const SDL_Color WHITE = { 255, 255, 255, 255 };

float inline lreLU(float x) { return(max(float(x / 10.0f), x)); }
float inline reLU(float x) { return(max(0.0f, x)); }
float inline sigmoid(float x) { return (1 / (1 + exp(-x))); }
float inline sigmoidD(float x) { float i = sigmoid(x); return i * (1 - i); }

float inline actF(float x) { return(reLU(x)); }
float inline sactF(float x) { return(tanh(x)); }

float inline genWeight() { return ((((float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018))) / 1018) / 2.5 - 1); }

template<typename t> int inline max(vector<t> val) { //Returns the largest element of a vector
    int m = 0;
    for (auto n : val) (n > m) ? m = n : m = m;
    return m;
}