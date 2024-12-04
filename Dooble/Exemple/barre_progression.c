#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include "option.h"

int main() {
    // Initialisation SDL + TTF (à retirer une fois "associé" avec le main, c est juste pour le test)
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Obtenir les dimensions de l'écran (idem, a retirer une fois "associé)
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    SDL_Window* window_regles = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_regles = SDL_CreateRenderer(window_regles , -1, SDL_RENDERER_ACCELERATED);

    // Création de la barre de progresssion
    SDL_Rect barre = {screen.w/5, screen.h/2, 1000, 200};

    // Début boucle principale
    int running = 1;
    SDL_Event event;
    int temps;
    int sub_barre_created = 0;  // Va permettre de vérifier si on a créé au moins une fois la barre de progression
    int changing_sub_barre = 0; // Va permettre de dire qu'on a modifié la position de la barre

    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if (click_in_rect(mouse_x, mouse_y, barre)) {
                    // Caclul du temps en fonction du click
                    float f_mouse_x = mouse_x, f_barre_x = barre.x, f_barre_w = barre.w;    // On passe tout en float pour éviter les pb de division entière
                    float tmp = 60 + (f_mouse_x-f_barre_x) / f_barre_w * 120;   // // tps de base + %barre * temps max à ajouter
                    temps = tmp ;   // On passe tmp (float) en int (temps)

                    sub_barre_created = 1;  // On dit qu'on a créer le sub_barre (utile uniquement la première fois)
                    changing_sub_barre = 1; // On dit qu'on a modifé la positon de la barre de progression
                }
            }
        }
        // Fin gestion des évenements


        // Remplisaage fond
        SDL_SetRenderDrawColor(renderer_regles, 0, 0, 0, 255);
        SDL_RenderClear(renderer_regles);

        // Remplissage de la barre en entier
        SDL_SetRenderDrawColor(renderer_regles, 150, 150, 150, 255);
        SDL_RenderFillRect(renderer_regles, &barre);

        // Remplissage de la barre pour afficher la progression (si elle existe)
        if (sub_barre_created) {
            int mouse_x, mouse_y;
            // Si on a changé la position de la barre, on modifie la position de la sourie (qui permet de définir les dimensions de la barre)
            // Sinon la barre reste la meme et est actualisé
            if (changing_sub_barre) {
                SDL_GetMouseState(&mouse_x, &mouse_y);
                changing_sub_barre = 0;
            }
            // On définit la barre (soit on la definit identique, ou soit on la definit avec les nouvelle dimensions), puis on la dessine
            SDL_Rect sub_barre = {barre.x, barre.y, mouse_x-barre.x, barre.h};
            SDL_SetRenderDrawColor(renderer_regles, 0, 255, 255, 255);
            SDL_RenderFillRect(renderer_regles, &sub_barre);
        }

        // Màj
        SDL_RenderPresent(renderer_regles);
    }
}
