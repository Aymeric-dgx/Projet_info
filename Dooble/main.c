#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    // Initialisation du SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Obtention de la taille de l'écran + affectation à des varaiables
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_height = screen.h;
    int screen_width = screen.w;

    //Création fenêtre + renderer (= moteur de rendu)
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height,
                                            SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Début création des strucures

    // Création d'un rectangle
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = screen_width / 2;
    rect.h = screen_height / 2;
    



    // Boucle principale
    int running = 1;
    SDL_Event event;
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }

        // Début code pour l'affichage "dynamique"

        // Set up fond en bleu
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderClear(renderer);

        // Set up rectangle en rouge
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
    }


    // Destruction propre du SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
