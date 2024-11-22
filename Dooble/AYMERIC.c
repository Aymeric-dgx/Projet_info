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
    SDL_Window* window1 = SDL_CreateWindow("first",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen.w, screen.h,
                                            SDL_WINDOW_SHOWN);
    SDL_Window* window2 = SDL_CreateWindow("second",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen.w, screen.h,
                                            SDL_WINDOW_SHOWN);
    SDL_Window* window3 = SDL_CreateWindow("third",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen.w, screen.h,
                                            SDL_WINDOW_HIDDEN);
    SDL_Renderer* renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer2 = SDL_CreateRenderer(window2, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* renderer3 = SDL_CreateRenderer(window3, -1, SDL_RENDERER_ACCELERATED);



    // Début création des strucures

    // Création d'un rectangle
    SDL_Rect rect;
    rect.w = screen_width / 2;
    rect.h = screen_height / 2;
    rect.x = screen.w/2 - rect.w/2;
    rect.y = screen.h/2 - rect.h/2;




    // Boucle principale
    int running = 1;
    int showWindow1 = 1;
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
                if (event.key.keysym.sym == SDLK_SPACE) {
                    showWindow1 = !showWindow1;
                    if (showWindow1) {
                        SDL_ShowWindow(window1);
                        SDL_HideWindow(window2);
                    } else {
                        SDL_ShowWindow(window2);
                        SDL_HideWindow(window1);
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                printf("Coordonnees du clic : %d %d\n", x, y);
                if (x>rect.x  &&  x<rect.x+rect.w  &&  y>rect.y  &&  y<rect.y+rect.h) {
                    if (SDL_GetWindowFlags(window1) & SDL_WINDOW_SHOWN  &&  SDL_GetWindowFlags(window2) & SDL_WINDOW_HIDDEN  &&  SDL_GetWindowFlags(window3) & SDL_WINDOW_HIDDEN) {  // Faire une fonction qui retournerai 1 si la bonne fenetre est montré et toute les autres sont caché ?
                        running = 0;
                    }
                }
            }
        }

        // Début code pour l'affichage "dynamique"

        // Recoloration des fenetres
        SDL_SetRenderDrawColor(renderer1, 0, 0, 255, 255);
        SDL_RenderClear(renderer1);
        SDL_SetRenderDrawColor(renderer2, 0, 255, 0, 255);
        SDL_RenderClear(renderer2);
        SDL_SetRenderDrawColor(renderer3, 255, 0, 0, 255);
        SDL_RenderClear(renderer3);

        // Set up rectangle en rouge dans window1
        SDL_SetRenderDrawColor(renderer1, 255, 150, 0, 255);
        SDL_RenderDrawRect(renderer1, &rect);
        SDL_RenderFillRect(renderer1, &rect);

        // Mise à jour fenetres
        SDL_RenderPresent(renderer1);
        SDL_RenderPresent(renderer2);
        SDL_RenderPresent(renderer3);

    }


    // Destruction propre du SDL
    SDL_DestroyRenderer(renderer1);
    SDL_DestroyRenderer(renderer2);
    SDL_DestroyRenderer(renderer3);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);
    SDL_DestroyWindow(window3);
    SDL_Quit();
    return 0;
}
