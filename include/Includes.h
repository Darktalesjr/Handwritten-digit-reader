#pragma once
#pragma warning (disable : 4244 267)

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <random>

#include "SDL_ttf.h"
#include "SDL.h"

using namespace std;

const int ROWS = 28;
const int COLS = 28;
const int ZOOM = 30;
const int ZOOM_ICON = 10;

const int IEXT = ROWS * COLS, OEXT = 10;
const int FACTDIM = 16, FACTDEPTH = 2;
const float LEARNING_RATE = 0.05f;
const int GROUP_SIZE = 1;

const int WIDTH = ZOOM * COLS, HEIGHT = ZOOM * ROWS;
const int WID = WIDTH / 2, HEI = HEIGHT / 2;

const SDL_Color WHITE = { 255, 255, 255, 255 };

float inline lreLU(float x) { return(max(float(x / 10.0f), x)); }
float inline lreLUD(float x) { return (x > 0) ? 1.0f : 0.1f; }
float inline reLU(float x) { return(max(0.0f, x)); }
float inline reLUD(float x) { return (x > 0) ? 1 : 0; }
float inline sigmoid(float x) { return (1 / (1 + exp(-x))); }

float inline sigmoidD(float x) { float s = sigmoid(x); return s * (1 - s); }
float inline tanhD(float x) { float i = 1 / cosh(x); return i * i; }

float inline actF(float x) { return(lreLU(x)); }
float inline actFD(float x) { return(lreLUD(x)); }
float inline sactF(float x) { return(sigmoid(x)); }
float inline sactFD(float x) { return(sigmoidD(x)); }

static random_device rd;
static mt19937 gen(rd());

float inline genWeight(int fanIn, int fanOut) {
    float stddev = sqrt(2.0 / (fanIn));
    normal_distribution<float> d(0, stddev);
    return d(gen);
}

float inline genWeightO(int fanIn, int fanOut) {
    float stddev = sqrt(6.0 / (fanIn + fanOut));
    normal_distribution<float> d(0, stddev);
    return d(gen);
}


template<typename t> int inline max(vector<t> val) {
    int m = 0;
    for (auto n : val) (n > m) ? m = n : m = m;
    return m;
}

template<typename t> int inline max(vector<vector<t>> val) { //Returns the largest size of a vector inside a vector of vectors
    int m = 0;
    for (vector<t> n : val) (n.size() > m) ? m = n.size() : m = m;
    return m;
}