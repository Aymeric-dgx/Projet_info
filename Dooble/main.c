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

    //Création de la fenêtre et du renderer  (nb : on peut créer plusieurs renderer si on souhaite)
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height,
                                            SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Définition de la couleur du fond (bleu)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);   // Fixe la couleur qui va être utlisié par ce renderer (pour modifier refaire un SetRenderDrawColor)
    SDL_RenderClear(renderer);


    // Boucle principale
    int running = 1;
    SDL_Event event;
    while(running) {
        // Gestion des évenements
        // SDL_PollEvent : renvoie 1 si un évenement en attente, sinon 0, et donc le code après la boucle peut s'éxecuter
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {                   // Si alt+f4 ou la croix rouge cliqué, running = 0  -> fin affichage
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {                // est vrai dès que n'importe quelle touche est pressée
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }

        // Début code pour l'affichage en tant que tel

        // Création d'un rectangle
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = screen_width / 2;
        rect.h = screen_height / 2;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // Définir la couleur du rectangle (rouge)
        SDL_RenderDrawRect(renderer, &rect);                        // Dessiner le rectangle (contour)
        SDL_RenderFillRect(renderer, &rect);                        // Remplir le rectangle
        SDL_RenderPresent(renderer);                                // Mettre à jour l'affichage
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
