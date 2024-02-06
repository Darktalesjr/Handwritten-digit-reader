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
    NeuralNet* net = new NeuralNet();
    thread trainThread(&NeuralNet::train, net);
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if (SDL_GetWindowFromID(event.window.windowID) == window) running = false;
                    else SDL_HideWindow(SDL_GetWindowFromID(event.window.windowID));
                    break;
                }
            }
        }
        renderDrawNet(renderer, net, font); 
    }
    trainThread.join();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


/*
#include "NeuralNetFunctions.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Window* netWindow = NULL;
    SDL_Renderer* netRenderer = NULL;
    TTF_Font* font;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("Global enviroment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("Resources/Fonts/Inconsolata_Condensed-SemiBold.ttf", 17);

    NeuralNet* net = new NeuralNet();
    //Automata* unit = new Automata[N]();
    //fstream file;
    //file.open("C:/Users/nicol/source/repos/AAIIbv2/Resources/source.bin", ios::in | ios::out | ios::app | ios::binary);
    //saveFile(net, &file);

    SDL_Event event;
    bool running = true, i = false;
    //thread* trainThread = new thread[12];
    //trainThread[0](&NeuralNet::train, net);
    while (running) {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    break;
                case SDL_WINDOWEVENT_CLOSE:
                    if (SDL_GetWindowFromID(event.window.windowID) == window) running = false;
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



        SDL_RenderPresent(renderer);
        if (i) renderDrawNet(netRenderer, net, font);
    }
    //trainThread.join();
    //file.close();
    SDL_DestroyRenderer(netRenderer);
    SDL_DestroyWindow(netWindow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}*/