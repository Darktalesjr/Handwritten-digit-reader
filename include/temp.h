#pragma once
#include "NeuralNetFunctions.h"

inline void renderDrawNet(SDL_Renderer* renderer, NeuralNet* net, TTF_Font* font)
{

    int wDiff = WIDTH / (net->activation.size() + 3);
    int hDiff = HEIGHT / (max(max(net->activation), OEXT));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    /*for (int ct1 = 0; ct1 < IEXT; ct1++) for (int ct = 0; ct < net->activation[0].size(); ct++)
    {
        (net->weight[0][ct][ct1] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight[0][ct][ct1])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight[0][ct][ct1]));
        SDL_RenderDrawLine(renderer, wDiff * 2, ((ct - (net->activation[0].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff, ((ct1 - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }*/

    for (int ct0 = 1; ct0 < net->activation.size(); ct0++) for (int ct = 0; ct < net->activation[ct0].size(); ct++)for (int ct1 = 0; ct1 < net->activation[ct0 - 1].size(); ct1++) {
        (net->weight[ct0][ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight[ct0][ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight[ct0][ct1][ct]));
        SDL_RenderDrawLine(renderer, wDiff * (ct0 + 2), ((ct - (net->activation[ct0].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff * (ct0 + 1), ((ct1 - (net->activation[ct0 - 1].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct = 0; ct < net->activation.back().size(); ct++)for (int ct1 = 0; ct1 < OEXT; ct1++)
    {
        (net->weight.back()[ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight.back()[ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight.back()[ct1][ct]));
        //SDL_RenderDrawLine(renderer, 0, 0 + ROWS * (ct1)+ct, 1, 1 + ROWS * (ct1)+ct); SDL_RenderDrawLine(renderer, 0, 0, WID, HEI);
        SDL_RenderDrawLine(renderer, WIDTH - wDiff * 2, ((ct - (net->activation.back().size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), WIDTH - wDiff, ((ct1 - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    SDL_RenderPresent(renderer);
}