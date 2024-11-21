#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <SDl_ttf.h>

int main(int argc, char* argv[]) {
    // Initialisation du SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Récupération de la taille de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_width = screen.w;
    int screen_height = screen.h;

    // Création fenetre + renderer
    SDL_Window* window1 = SDL_CreateWindow(   "Dooble",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        400, 400,
                        SDL_WINDOW_SHOWN);
    SDL_Window* window2 = SDL_CreateWindow(   "test",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        800, 200,
                        SDL_WINDOW_HIDDEN);
    SDL_Renderer* renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED);

    // Début mise en forme initiale des fenetres
    SDL_SetRenderDrawColor(renderer1, 255, 0, 255, 255);
    SDL_RenderClear(renderer1);
    SDL_RenderPresent(renderer1);

    // Création d un carré
    SDL_Rect rect = {100, 100, 200, 200};
    SDL_SetRenderDrawColor(renderer1, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer1, &rect);




    // Boucle principale
    int running = 1;
    int showWindow = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    showWindow = !showWindow;
                    if (showWindow) {
                        SDL_ShowWindow(window1);
                        SDL_HideWindow(window2);
                    } else {
                        SDL_HideWindow(window1);
                        SDL_ShowWindow(window2);
                    }

                }
            }
        }
        // Debut affichage dynamique
        SDL_RenderPresent(renderer1);

    }

    // Destruction propre du SDL
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);
    SDL_Quit();
}
