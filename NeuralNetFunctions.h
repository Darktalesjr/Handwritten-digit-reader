#include "includes.h"

#include "automata.h"
#include "SDL_ttf.h"

TTF_Font* font;
const SDL_Color WHITE = { 255, 255, 255, 255 };

float reLU(float x) { return(max(float(x / 10), x)); }
float sigmoid(float x) { return (1 / (1 + exp(-x))); }

float actF(float x) { return(reLU(x)); }
float sactF(float x) { return(tanh(x)); }

template<typename t> int max(vector<t> val) { //Returns the largest element of a vector
    int m = 0;
    for (auto n : val) (n > m) ? m = n : m = m;
    return m;
}

void drawCircle(SDL_Renderer* renderer, int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    while (y >= x)
    {
        SDL_RenderDrawPoint(renderer, xc + x, yc + y);
        SDL_RenderDrawPoint(renderer, xc - x, yc + y);
        SDL_RenderDrawPoint(renderer, xc + x, yc - y);
        SDL_RenderDrawPoint(renderer, xc - x, yc - y);
        SDL_RenderDrawPoint(renderer, xc + y, yc + x);
        SDL_RenderDrawPoint(renderer, xc - y, yc + x);
        SDL_RenderDrawPoint(renderer, xc + y, yc - x);
        SDL_RenderDrawPoint(renderer, xc - y, yc - x);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y);
        }
        else d = d + 4 * x;
    }
}

void drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius)
{
    int r2 = radius * radius;
    int area = r2 << 2;
    int rr = radius << 1;

    for (int i = 0; i < area; i++)
    {
        int tx = (i % rr) - radius;
        int ty = (i / rr) - radius;

        if (tx * tx + ty * ty <= r2) SDL_RenderDrawPoint(renderer, centreX + tx, centreY + ty);
    }
}

void renderDrawNet(SDL_Renderer* renderer, SDL_Window* window, Automata* unit)
{
    int wDiff = WIDTH / (unit->neuralNet.actDim.size() + 3);
    int hDiff = HEIGHT / (max(max(max(unit->neuralNet.actDim), int(unit->neuralNet.input.size())), int(unit->neuralNet.output.size())));

    string sSource;
    const char* source = sSource.c_str();
    SDL_Surface* surfaceMessage;
    SDL_Texture* text;
    SDL_Rect messageRect;
    int w, h;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    for (int ct1 = 0; ct1 < IEXT; ct1++) for (int ct = 0; ct < unit->neuralNet.actDim[0]; ct++) {
        (unit->neuralNet.weight[0][ct][ct1] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(unit->neuralNet.weight[0][ct][ct1])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(unit->neuralNet.weight[0][ct][ct1]));
        SDL_RenderDrawLine(renderer, wDiff * 2, ((ct - (unit->neuralNet.actDim[0] / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff, ((ct1 - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct0 = 1; ct0 < unit->neuralNet.actDim.size(); ct0++) for (int ct = 0; ct < unit->neuralNet.actDim[ct0]; ct++)for (int ct1 = 0; ct1 < unit->neuralNet.actDim[ct0 - 1]; ct1++) {
        (unit->neuralNet.weight[ct0][ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(unit->neuralNet.weight[ct0][ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(unit->neuralNet.weight[ct0][ct1][ct]));
        SDL_RenderDrawLine(renderer, wDiff * (ct0 + 2), ((ct - (unit->neuralNet.actDim[ct0] / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff * (ct0 + 1), ((ct1 - (unit->neuralNet.actDim[ct0 - 1] / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct = 0; ct < unit->neuralNet.actDim.back(); ct++)for (int ct1 = 0; ct1 < OEXT; ct1++) {
        (unit->neuralNet.weight.back()[ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(unit->neuralNet.weight.back()[ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(unit->neuralNet.weight.back()[ct1][ct]));
        SDL_RenderDrawLine(renderer, WIDTH - wDiff * 2, ((ct - (unit->neuralNet.actDim.back() / 2.0f)) * hDiff) + HEI + (hDiff / 2), WIDTH - wDiff, ((ct1 - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    for (int ct = 0; ct < IEXT; ct++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, wDiff, ((ct - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, wDiff, ((ct - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
    }

    for (int ct = 0; ct < unit->neuralNet.actDim.size(); ct++)for (int ct1 = 0; ct1 < unit->neuralNet.actDim[ct]; ct1++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, wDiff * (ct + 2), ((ct1 - (unit->neuralNet.actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, wDiff * (ct + 2), ((ct1 - (unit->neuralNet.actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);

        messageRect = { wDiff * (ct + 2), ((ct1 - (unit->neuralNet.actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2), w,h };
        surfaceMessage = TTF_RenderText_Solid(font, source, WHITE);
        text = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_RenderCopy(renderer, text, NULL, &messageRect);
    }

    for (int ct = 0; ct < OEXT; ct++) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, WIDTH - wDiff, ((ct - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, WIDTH - wDiff, ((ct - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
    }
}

void evolve(Automata* unit)
{
    sort(unit);
    Automata* p1, * p2;
    for (int ct0 = FITTEST; ct0 < N; ct0++)
    {
        p1 = &unit[rand() % FITTEST];
        p2 = &unit[rand() % FITTEST];

        int ma = p1->neuralNet.actDim.size(), mi = p2->neuralNet.actDim.size();
        if (p2->neuralNet.actDim.size() > p1->neuralNet.actDim.size()) ma = p1->neuralNet.actDim.size(), mi = p2->neuralNet.actDim.size(), swap(p1, p2);
        //unit[ct0].neuralNet.actDim.resize(ma);
    }
}

/*
if (bool(rand() % 2))
        {
            unit[ct0].neuralNet.actDim[0] = p1->neuralNet.actDim[0];
            unit[ct0].neuralNet.activation[0].resize(unit[ct0].neuralNet.actDim[0]);
            unit[ct0].neuralNet.weight[0].resize(unit[ct0].neuralNet.actDim[0]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[0]; ct1++)
            {
                for (int ct2 = 0; ct2 < IEXT; ct2++)
                {
                    if (bool(rand() % 2) && 0 < p1->neuralNet.activation[0].size())
                    {
                        unit[ct0].neuralNet.weight[0][ct1][ct2] = p2->neuralNet.weight[0][ct1][ct2];
                    }
                    else unit[ct0].neuralNet.weight[0][ct1][ct2] = p1->neuralNet.weight[0][ct1][ct2];
                }
                unit[ct0].neuralNet.bias[0][ct1] = p1->neuralNet.bias[0][ct1];
            }
        }
        else
        {
            unit[ct0].neuralNet.actDim[0] = p2->neuralNet.actDim[0];
            unit[ct0].neuralNet.activation[0].resize(unit[ct0].neuralNet.actDim[0]);
            unit[ct0].neuralNet.weight[0].resize(unit[ct0].neuralNet.actDim[0]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[0]; ct1++)
            {
                for (int ct2 = 0; ct2 < IEXT; ct2++)
                {
                    if (bool(rand() % 2) && 0 < p1->neuralNet.activation[0].size())
                    {
                        unit[ct0].neuralNet.weight[0][ct1][ct2] = p2->neuralNet.weight[0][ct1][ct2];
                        unit[ct0].neuralNet.bias[0][ct1] = p2->neuralNet.bias[0][ct1];
                    }
                    else unit[ct0].neuralNet.weight[0][ct1][ct2] = p1->neuralNet.weight[0][ct1][ct2];
                    unit[ct0].neuralNet.bias[0][ct1] = p1->neuralNet.bias[0][ct1];
                }
                unit[ct0].neuralNet.bias[0][ct1] = p2->neuralNet.bias[0][ct1];
            }
        }
        for (int ct = 1; ct < mi; ct++)
        {
            if (bool(rand() % 2)) unit[ct0].neuralNet.actDim[ct] = p1->neuralNet.actDim[ct];
            else unit[ct0].neuralNet.actDim[ct] = p2->neuralNet.actDim[ct];
            unit[ct0].neuralNet.activation[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            unit[ct0].neuralNet.weight[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[ct]; ct1++)
            {
                unit[ct0].neuralNet.weight[ct][ct1].resize(unit[ct0].neuralNet.actDim[ct]);
                for (int ct2 = 0; ct2 < unit[ct0].neuralNet.actDim[ct - 1]; ct2++)
                {
                    if (ct1 < p2->neuralNet.activation[ct].size() && ct2 < p2->neuralNet.weight[ct][ct1].size())
                    {
                        if (ct1 < p1->neuralNet.activation[ct].size() && ct2 < p1->neuralNet.weight[ct][ct1].size())
                        {
                            if (bool(rand() % 2)) { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p2->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                            else { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                        }
                        else { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p2->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p2->neuralNet.bias[ct][ct1]; }
                    }
                    else{ unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p1->neuralNet.bias[ct][ct1]; }
                }
            }
        }
        for (int ct = mi; ct < ma; ct++)
        {
            unit[ct0].neuralNet.actDim[ct] = p1->neuralNet.actDim[ct];
            unit[ct0].neuralNet.activation[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            unit[ct0].neuralNet.weight[ct].resize(unit[ct0].neuralNet.actDim[ct]);
            for (int ct1 = 0; ct1 < unit[ct0].neuralNet.actDim[ct]; ct1++)
            {
                unit[ct0].neuralNet.weight[ct][ct1].resize(unit[ct0].neuralNet.actDim[ct]);
                for (int ct2 = 0; ct2 < unit[ct0].neuralNet.actDim[ct - 1]; ct2++) { unit[ct0].neuralNet.weight[ct][ct1][ct2] = p1->neuralNet.weight[ct][ct1][ct2]; unit[ct0].neuralNet.bias[ct][ct1] = p1->neuralNet.bias[ct][ct1];}
            }
        }
        */
