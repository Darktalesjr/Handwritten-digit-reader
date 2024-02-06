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

const uint32_t ROWS = 28;
const uint32_t COLS = 28;
const int ZOOM = 30;
const int ZOOM_ICON = 10;

const int IEXT = ROWS * 28, OEXT = 10;
const int FACTDIM = 16, FACTDEPTH = 2;
const float LEARNINGRATE = 1.0f;

const int WIDTH = ZOOM * COLS, HEIGHT = ZOOM * ROWS;
const int WID = WIDTH / 2, HEI = HEIGHT / 2;

const SDL_Color WHITE = { 255, 255, 255, 255 };

float inline lreLU(float x) { return(max(float(x / 10.0f), x)); }
float inline lreLUD(float x) { return (x > 0) ? 0.1f : 1; }
float inline reLU(float x) { return(max(0.0f, x)); }
float inline reLUD(float x) { return (x > 0) ? 0 : 1; }
float inline sigmoid(float x) { return (1 / (1 + exp(-x))); }
float inline sigmoidD(float x) { float i = exp(x); return i / (1 + i) * (1 + i); }
float inline tanhD(float x) { float i = 1 / cosh(x); return i * i; }

float inline actF(float x) { return(lreLU(x)); }
float inline actFD(float x) { return(lreLUD(x)); }
float inline sactF(float x) { return(tanh(x)); }
float inline sactFD(float x) { return(tanhD(x)); }

float inline genWeight() { return ((((float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018) + float(rand() % 1018))) / 1018) / 2.5 - 1); }

template<typename t> int inline max(vector<t> val) { //Returns the largest element of a vector
    int m = 0;
    for (auto n : val) (n > m) ? m = n : m = m;
    return m;
}

template<typename t> int inline max(vector<vector<t>> val) { //Returns the largest size of a vector inside a vector of vectors
    int m = 0;
    for (vector<t> n : val) (n.size() > m) ? m = n.size() : m = m;
    return m;
}