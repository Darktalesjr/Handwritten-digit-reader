#include "includes.h"
#include "SDL.h"
#include "Automata.h"
#include "NeuralNetFunctions.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CollisionTest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    automata* unit = new automata[N]();
    player user;
    user.initPlayer();
    SDL_Rect objRect{ 0,0,EXT,EXT };
    SDL_Event event;
    bool exit = true;
    bool w = false, a = false, s = false, d = false;

    while (exit) {
        for (int genTime = 0; genTime < 500; genTime++)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    exit = true;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        break;
                    }
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        w = true;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        a = true;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        s = true;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        d = true;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        w = false;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        a = false;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        s = false;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        d = false;
                        break;
                    default:
                        break;
                    }
                }
            }
            if (w) if (user.y == L) user.ay = 11;
            if (a) user.x -= 3;
            //if (s);
            if (d) user.x += 3;

            //p = SDL_GetMouseState(&mx, &my);
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
                try {
                    unit[ct].act();
                }
                catch (...) {
                    cout << "wha";
                }

                if (unit[ct].player.x < TXE)unit[ct].player.x = TXE;
                if (unit[ct].player.x > WIDTH - TXE)unit[ct].player.x = WIDTH - TXE;
                objRect.x = unit[ct].player.x - TXE, objRect.y = unit[ct].player.y - TXE;
                SDL_RenderDrawRect(renderer, &objRect);
            }

            user.y -= user.ay;
            if (user.y <= L) user.ay -= 0.5;
            else user.y = L;

            if (user.x < TXE) user.x = TXE;
            if (user.x > WIDTH - TXE)user.x = WIDTH - TXE;
            objRect.x = user.x - TXE, objRect.y = user.y - TXE;
            SDL_RenderDrawRect(renderer, &objRect);
            SDL_RenderPresent(renderer);
        }

        for (int ct = 0; ct < N; ct++)unit[ct].neuralNet.fitness = GOAL[0] - unit[ct].player.x;
        //for (int ct = 0; ct < N; ct++){cout << unit[ct].neuralNet.fitness << " - ";}
        for (int ct = 1; ct < N;)
        {
            if (unit[ct].neuralNet.fitness < unit[ct - 1].neuralNet.fitness) {
                swap(&unit[ct], &unit[ct - 1]);
                if (ct > 0) ct--;
            }
            else ct++;
        }
        //for (int ct = 0; ct < N; ct++){unit[ct].player.initPlayer();}
        //evolve(unit);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}