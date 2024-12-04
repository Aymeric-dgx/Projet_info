// Aymeric : je trouve c est juste un peu moins obscure et plus clair que celui de chatGPT, a vous de me dire

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"
#include <stdio.h>


int main() {
    // Initialisation SDL + TTF (à retirer une fois "associé" avec le main, c est juste pour le test)
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Obtenir les dimensions de l'écran (idem, a retirer une fois "associé)
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);


    // Création fenetre + renderer + chargement police
    SDL_Window* window_menu = SDL_CreateWindow("Menu",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen.w, screen.h,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_menu = SDL_CreateRenderer(window_menu, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* bigFont = TTF_OpenFont("../police/arial.ttf", (screen.w / 250) * 24); // Police grande taille (taille qui s'adapte à la taille de l'écan)
    TTF_Font* smallFont = TTF_OpenFont("../police/arial.ttf", (screen.w / 450) * 24); // Police petite taille (taille qui s'adapte à la taille de l'écan)


    // Texte "Dooble" : surface + texture + position (rectangle)
    SDL_Color tittleTextColor = {255, 255, 255, 255};   // Blanc
    SDL_Surface* tittleSurface = TTF_RenderText_Solid(bigFont, "Dooble", tittleTextColor);
    SDL_Texture* tittleTexture = SDL_CreateTextureFromSurface(renderer_menu, tittleSurface);
    SDL_Rect titleRect = {screen.w/2 - tittleSurface->w/2, screen.h/8 - tittleSurface->h/2.5, tittleSurface->w, tittleSurface->h};
    SDL_FreeSurface(tittleSurface);

    
    // Création des boutons Play, Score, Quit
    SDL_Color buttonTextColor = {0, 0, 0, 255}; // Noir
    char* buttonNames[3] = {"Play", "Score", "Quit"};   // Pointeur de chaine de caractère
    SDL_Rect buttonRects[3];    // Tableau des rectangles pour les boutons
    SDL_Rect textButtonRects[3];    // Tableau des rectanles pour le placement du texte (pour les placer dans les gros rectangles qui feront le fond des boutons)
    SDL_Texture* buttonTextures[3]; // Tableau pour stocker les textures pour les boutons

    for(int i=0 ; i<3 ; i++) {
        // Création du texte des boutons
        SDL_Surface* buttonSurface = TTF_RenderText_Solid(smallFont, buttonNames[i], buttonTextColor);  // police, texte, couleur
        buttonTextures[i] = SDL_CreateTextureFromSurface(renderer_menu, buttonSurface); // renderer(on va dire la fenetre), surface

        // Placement des boutons (= "gros rectangles")
        buttonRects[i].w = screen.w / 3;
        buttonRects[i].h = screen.h/ 6.5;
        buttonRects[i].x = screen.w/2 - buttonRects[i].w/2;
        buttonRects[i].y = buttonRects[i].h*3 + i*screen.h/5.5 - screen.h/8;

        // Placement des "petits rectangle" dans les boutons
        textButtonRects[i].w = buttonSurface->w;
        textButtonRects[i].h = buttonSurface->h;
        textButtonRects[i].x = buttonRects[i].x + (buttonRects[i].w - textButtonRects[i].w)/2;
        textButtonRects[i].y = buttonRects[i].y + (buttonRects[i].h - textButtonRects[i].h)/2;

        SDL_FreeSurface(buttonSurface);
    }


    // Boucle principale
    int returning = -1;
    int running = 1;
    SDL_Event event;

    while(running) {
        // Gestion des événements
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if (click_in_rect(mouse_x, mouse_y, buttonRects[0])) {  // Bouton "Play" cliqué
                    printf("Bouton Play cliqué\n");
                    returning = 1;
                }
                else if (click_in_rect(mouse_x, mouse_y, buttonRects[1])) {  // Bouton "Scores" cliqué
                    printf("Bouton Scores cliqué\n");
                    returning = 2;
                }
                else if (click_in_rect(mouse_x, mouse_y, buttonRects[2])) {  // Bouton "Quitter" cliqué
                    printf("Bouton Quitter cliqué\n");
                    returning = 0;
                    running = 0;
                }
            }
        }
        // Fin gestion des évements

        //Peinture du fond
        SDL_SetRenderDrawColor(renderer_menu,0,0,50,255);
        SDL_RenderClear(renderer_menu);

        // Dessiner le texte principal "Dooble"
        SDL_RenderCopy(renderer_menu, tittleTexture, NULL, &titleRect);

        // Dessiner les boutons
        for (int i=0 ; i<3 ; i++) {
            SDL_SetRenderDrawColor(renderer_menu,255,255,255,255);  // Blanc
            SDL_RenderFillRect(renderer_menu, &buttonRects[i]);             // Remplir les "gros" carré --> le fond des boutons
            SDL_SetRenderDrawColor(renderer_menu,0,0,0,255);       // Noir
            SDL_RenderCopy(renderer_menu, buttonTextures[i], NULL, &textButtonRects[i]);    // "Imprimer"" le texte
        }

        // Mis à jour de l'écran
        SDL_RenderPresent(renderer_menu);

        printf("Returning = %d\n", returning);
    }
    // Fin boucle principale

    if (returning != -1) {
        return returning;
    } else {
        printf("pb avec le returning");
        return -1;
    }
}
