#include "NeuralNetFunctions.h"

inline void drawCircle(SDL_Renderer* renderer, int xc, int yc, int r)
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

inline void drawFilledCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius)
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

    int wDiff = WIDTH / (net->activation.size() + 3);
    int hDiff = HEIGHT / (max(max(net->activation), OEXT));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    for (int ct0 = 1; ct0 < net->activation.size(); ct0++) for (int ct = 0; ct < net->activation[ct0].size(); ct++)for (int ct1 = 0; ct1 < net->activation[ct0 - 1].size(); ct1++) {
        (net->weight[ct0][ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight[ct0][ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight[ct0][ct1][ct]));
        SDL_RenderDrawLine(renderer, wDiff * (ct0 + 2), ((ct - (net->activation[ct0].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), wDiff * (ct0 + 1), ((ct1 - (net->activation[ct0 - 1].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    for (int ct = 0; ct < net->activation.back().size(); ct++)for (int ct1 = 0; ct1 < OEXT; ct1++)
    {
        (net->weight.back()[ct1][ct] > 0) ? SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 * fabsf(net->weight.back()[ct1][ct])) : SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255 * fabsf(net->weight.back()[ct1][ct]));
        SDL_RenderDrawLine(renderer, WIDTH - wDiff * 2, ((ct - (net->activation.back().size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), WIDTH - wDiff, ((ct1 - (OEXT / 2.0f)) * hDiff) + HEI + (hDiff / 2));
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    for (int ct = 0; ct < ROWS; ct++) for (int ct1 = 0; ct1 < COLS; ct1++) {
        SDL_SetRenderDrawColor(renderer, net->input[ct*ROWS+ct1] * 255, net->input[ct * ROWS + ct1] * 255, net->input[ct * ROWS + ct1] * 255, 255);
        for (int ct2 = 0; ct2 < ZOOM_ICON; ct2++)for (int ct3 = 0; ct3 < ZOOM_ICON; ct3++) SDL_RenderDrawPoint(renderer, ct1 * ZOOM_ICON + ct2 + (wDiff*2 - ZOOM_ICON * ROWS) / 2, ct * ZOOM_ICON + ct3 + HEI - ZOOM_ICON * ROWS / 2);
    }

    string source;
    SDL_Surface* surfaceMessage;
    SDL_Texture* text;
    const char* sourcePtr = source.c_str();
    SDL_Rect messageRect;
    int w = 0, h = 0;

    for (int ct = 0; ct < net->activation.size(); ct++)for (int ct1 = 0; ct1 < net->activation[ct].size(); ct1++)
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawFilledCircle(renderer, wDiff * (ct + 2), ((ct1 - (net->activation[ct].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawCircle(renderer, wDiff * (ct + 2), ((ct1 - (net->activation[ct].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2), 20);

        source = to_string(float(net->activation[ct][ct1])).substr(0, 4);
        TTF_SizeText(font, sourcePtr, &w, &h);
        surfaceMessage = TTF_RenderText_Solid(font, sourcePtr, WHITE);
        text = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        messageRect = { wDiff * (ct + 2) - w / 2, int((ct1 - (net->activation[ct].size() / 2.0f)) * hDiff) + HEI + (hDiff / 2) - h / 2 ,w,h };
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
