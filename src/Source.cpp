#include "includes.h"
#include "NeuralNetFunctions.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font* font;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("Global enviroment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    font = TTF_OpenFont("Resources/Fonts/Inconsolata_Condensed-SemiBold.ttf", 17);

    SDL_Event event;

    bool exit = true, i = false;

    NeuralNet* net = new NeuralNet();
    thread trainThread(&NeuralNet::train, net);

    while (exit) {

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
                    if (SDL_GetWindowFromID(event.window.windowID) == window) exit = false;
                    else SDL_HideWindow(SDL_GetWindowFromID(event.window.windowID));
                    break;
                }
            }
        }
        renderDrawNet(renderer, net, font);
        cout << "-------------------------------------------------------------------------------";
    }
    trainThread.join();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
