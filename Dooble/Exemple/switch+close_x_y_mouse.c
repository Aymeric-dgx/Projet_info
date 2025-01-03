// Fenetre 1 (F1) : avec le rectangle      Fenetre 2 (F2) : sans rien
// Programme stop si clique dans le rectangle du F1
// Quand on switch pour cacher F1 et montrer F2, si on clique dans la zone ou il y avait le recantgel le programme se ferme
// Faire condition pour verifier le x et y de la souris MAIS AUSSI verifier que la bonne fenetre est ouverte

// Sol° -> Utiliser SDL_GetWindowFlags(window) -> récupere le flag (=option) de la fenetre, qu'on pourra vérifier si = à SDL_WINDOW_SHOWN ou = à SDL_WINDOW_HIDDEN
// Attention : SDL_GetWindowFlags renvoie un masque (= liste) de bits, on peu pas comparer directement SDL_GetWindowFlags == SDL_WINDOW_SHOWN
// Il faut faire un ET logique (&) pour comparer les deux masques

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
    SDL_Window* window1 = SDL_CreateWindow("Dooble",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height,
                                            SDL_WINDOW_SHOWN);
    SDL_Window* window2 = SDL_CreateWindow("test",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height,
                                            SDL_WINDOW_HIDDEN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);

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
                if (x>rect.x  &&  x<rect.x+rect.w  &&  y>rect.y  &&  y<rect.y+rect.h && (SDL_GetWindowFlags(window1) & SDL_WINDOW_SHOWN) ) {    // () pas obligatoire, juste visuel
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

        // Mise à jour fenetre
        SDL_RenderPresent(renderer);
    }


    // Destruction propre du SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window1);
    SDL_DestroyWindow(window2);
    SDL_Quit();
    return 0;
}
