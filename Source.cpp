#include "includes.h"
#include "SDL.h"
#include "Automata.h"
#include "NeuralNetFunctions.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Window* netWindow = NULL;
    SDL_Renderer* netRenderer = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Global enviroment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("Resources/Fonts/Inconsolata_Condensed-SemiBold.ttf", 17);

    Automata* unit = new Automata[N]();
    SDL_Rect objRect{ 0,0,EXT,EXT };
    SDL_Event event;

    bool exit = true, w = false, a = false, s = false, d = false, i = false;

    while (exit) {
        for (int genTime = 0; genTime < GENTIMEMAX; genTime++)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    exit = false;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        if (SDL_GetWindowFromID(event.window.windowID) == window) { exit = false; genTime = GENTIMEMAX; }
                        else SDL_HideWindow(SDL_GetWindowFromID(event.window.windowID));
                        break;
                    }
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_I:
                        i = !i;
                        if (i) {
                            netWindow = SDL_CreateWindow("Net renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
                            netRenderer = SDL_CreateRenderer(netWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                        }
                        else {
                            SDL_DestroyRenderer(netRenderer);
                            SDL_DestroyWindow(netWindow);
                        }

                        break;
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            SDL_RenderDrawLine(renderer, 0, LIMIT, WIDTH, LIMIT);

            for (int ct = 0; ct < N; ct++)
            {
                unit[ct].player.y -= unit[ct].player.ay;
                if (unit[ct].player.y <= L) unit[ct].player.ay -= 0.5;
                else unit[ct].player.y = L;

                unit[ct].think();
                unit[ct].act();

                if (unit[ct].player.x < TXE)unit[ct].player.x = TXE;
                if (unit[ct].player.x > WIDTH - TXE)unit[ct].player.x = WIDTH - TXE;
                objRect.x = unit[ct].player.x - TXE, objRect.y = unit[ct].player.y - TXE;
                SDL_RenderDrawRect(renderer, &objRect);
            }
            if (i) renderDrawNet(netRenderer, &unit[0]);
            SDL_RenderPresent(renderer);
        }
        //for (int ct = 0; ct < N; ct++)
        {
            
        }

        for (int ct = 0; ct < N; ct++)unit[ct].neuralNet.fitness = abs(GOAL[0] - unit[ct].player.x + 200 -unit[ct].player.j * 200);
        evolve(unit);
        for (int ct = 0; ct < N; ct++)unit[ct].player.initPlayer();
        for (int ct = 0; ct < N; ct++)cout << unit[ct].neuralNet.fitness << " - "; cout << endl;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
