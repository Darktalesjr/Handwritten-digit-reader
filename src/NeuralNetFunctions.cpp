#include "NeuralNetFunctions.h"

void inline drawCircle(SDL_Renderer* renderer, int xc, int yc, int r)
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

void inline drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius)
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

void renderDrawNet(SDL_Renderer* renderer, NeuralNet* net, TTF_Font* font)
{
    int wDiff = WIDTH / (net->actDim.size() + 3);
    int hDiff = HEIGHT / (max(max(max(net->actDim), int(net->input.size())), int(net->output.size())));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    for (int ct1 = 0; ct1 < IEXT; ct1++) for (int ct = 0; ct < net->actDim[0]; ct++)
    {
        (net->weight[0][ct][ct1] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight[0][ct][ct1])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight[0][ct][ct1]));
        SDL_RenderDrawLine(renderer, wDiff * 2, ((ct - (net->actDim[0] / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff, ((ct1 - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct0 = 1; ct0 < net->actDim.size(); ct0++) for (int ct = 0; ct < net->actDim[ct0]; ct++)for (int ct1 = 0; ct1 < net->actDim[ct0 - 1]; ct1++) {
        (net->weight[ct0][ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight[ct0][ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight[ct0][ct1][ct]));
        SDL_RenderDrawLine(renderer, wDiff * (ct0 + 2), ((ct - (net->actDim[ct0] / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff * (ct0 + 1), ((ct1 - (net->actDim[ct0 - 1] / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct = 0; ct < net->actDim.back(); ct++)for (int ct1 = 0; ct1 < OEXT; ct1++)
    {
        (net->weight.back()[ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight.back()[ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight.back()[ct1][ct]));
        SDL_RenderDrawLine(renderer, WIDTH - wDiff * 2, ((ct - (net->actDim.back() / 2.0f)) * hDiff) + HEI + (hDiff / 2), WIDTH - wDiff, ((ct1 - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    string source;
    SDL_Surface* surfaceMessage;
    SDL_Texture* text;
    const char* sourcePtr = source.c_str();
    SDL_Rect messageRect;
    int w = 0, h = 0;

    for (int ct = 0; ct < IEXT; ct++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, wDiff, ((ct - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, wDiff, ((ct - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);

        source = to_string(int(net->input[ct]));
        TTF_SizeText(font, sourcePtr, &w, &h);
        surfaceMessage = TTF_RenderText_Solid(font, sourcePtr, WHITE);
        text = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        messageRect = { wDiff - w / 2 , int((ct - (IEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2) - h / 2 ,w,h };
        SDL_RenderCopy(renderer, text, NULL, &messageRect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(text);
    }

    for (int ct = 0; ct < net->actDim.size(); ct++)for (int ct1 = 0; ct1 < net->actDim[ct]; ct1++)
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, wDiff * (ct + 2), ((ct1 - (net->actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, wDiff * (ct + 2), ((ct1 - (net->actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);

        source = to_string(float(net->activation[ct][ct1])).substr(0, 4);
        TTF_SizeText(font, sourcePtr, &w, &h);
        surfaceMessage = TTF_RenderText_Solid(font, sourcePtr, WHITE);
        text = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        messageRect = { wDiff * (ct + 2) - w / 2, int((ct1 - (net->actDim[ct] / 2.0f)) * hDiff) + HEI + (hDiff / 2) - h / 2 ,w,h };
        SDL_RenderCopy(renderer, text, NULL, &messageRect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(text);
    }

    for (int ct = 0; ct < OEXT; ct++)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, WIDTH - wDiff, ((ct - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, WIDTH - wDiff, ((ct - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);

        source = to_string(float(net->output[ct])).substr(0, 4);
        TTF_SizeText(font, sourcePtr, &w, &h);
        surfaceMessage = TTF_RenderText_Solid(font, sourcePtr, WHITE);
        text = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        messageRect = { WIDTH - wDiff - w / 2, int((ct - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2) - h / 2,w,h };
        SDL_RenderCopy(renderer, text, NULL, &messageRect);
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(text);
    }
    SDL_RenderPresent(renderer);
}

void saveFile(NeuralNet* source, fstream* file) // WIP
{

}
