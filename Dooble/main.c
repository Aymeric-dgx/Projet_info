#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
kjgvhv
    // Initialisation du SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    // Obtention de la taille de l'écran + affectation à des varaiables
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_height = screen.h;
    int screen_width = screen.w;

    //Création de la fenêtre et du renderer (= moteur de rendu)  (nb : on peut créer plusieurs renderer si on souhaite)
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height,
                                            SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    //Début mise en forme initial des fenêtres

    // Set up fond en bleu
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);   // Fixe la couleur qui va être utlisié par ce renderer (pour modifier refaire un SetRenderDrawColor)
    SDL_RenderClear(renderer);                                  //  Efface la cible de rendu actuelle avec la couleur de dessin définie

    // Création d'un rectangle
    SDL_Rect rect;
    rect.x = 0;                                                 // Position coin haut gauche en x
    rect.y = 0;                                                 // Position coin haut gauche en y
    rect.w = screen_width / 2;                                  // Largeur du rectangle
    rect.h = screen_height / 2;                                 // Hauteur du rectangle
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // Définir la couleur du rectangle (rouge)
    SDL_RenderDrawRect(renderer, &rect);                        // Dessiner le rectangle (contour)
    SDL_RenderFillRect(renderer, &rect);                        // Remplir le rectangle
    SDL_RenderPresent(renderer);                                // Mettre à jour l'affichage





    // Boucle principale
    int running = 1;
    SDL_Event event;
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {                      // SDL_PollEvent : renvoie 1 si un évenement en attente, sinon 0, et donc le code après la boucle peut s'éxecuter
            if (event.type == SDL_QUIT) {                   // Si alt+f4 ou la croix rouge cliqué, running = 0  -> fin affichage
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {                // est vrai dès que n'importe quelle touche est pressée
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }

        // Début code pour l'affichage "dynamique"

    }

    
    // Destruction propre du SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
