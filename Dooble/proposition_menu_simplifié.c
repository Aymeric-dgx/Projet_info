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


    // Création fenetre + renderer + chargement police (qui s'adaptera à l'écran)
    SDL_Window* window_menu = SDL_CreateWindow("Menu",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen.w, screen.h,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_menu = SDL_CreateRenderer(window_menu, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* bigFont = TTF_OpenFont("../police/arial.ttf", (screen.w / 250) * 24); // Police grande taille
    TTF_Font* smallFont = TTF_OpenFont("../police/arial.ttf", (screen.w / 450) * 24); // Police petite taille

    // Création des boutons

    SDL_Color button_text_color = {0,0,0,255};
    SDL_Color button_color = {255,255,255,255};
    SDL_Rect button_rects[3];
    SDL_Rect text_button_rects[3];
    char* button_names[3] = {"Play", "Scores", "Quit"};

    for(int i=0 ; i<3 ; i++) {

        // Placement des boutons (= "gros rectangles")
        button_rects[i].w = screen.w / 3;
        button_rects[i].h = screen.h/ 6.5;
        button_rects[i].x = screen.w/2 - button_rects[i].w/2;
        button_rects[i].y = button_rects[i].h*3 - screen.h/8 + i*screen.h/5.5;    // Espacement vertical entre chaque boutons

        // Placement des "petits rectangle" dans les boutons
        SDL_Surface* tmp_surface = TTF_RenderText_Solid(smallFont, button_names[i], button_text_color);
        text_button_rects[i].w = tmp_surface->w;
        text_button_rects[i].h = tmp_surface->h;
        text_button_rects[i].x = button_rects[i].x + (button_rects[i].w - text_button_rects[i].w)/2;
        text_button_rects[i].y = button_rects[i].y + (button_rects[i].h - text_button_rects[i].h)/2;
        SDL_FreeSurface(tmp_surface);
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
                if (click_in_rect(mouse_x, mouse_y, button_rects[0])) {  // Bouton "Play" cliqué
                    printf("Bouton Play clique\n");
                    returning = 1;
                }
                else if (click_in_rect(mouse_x, mouse_y, button_rects[1])) {  // Bouton "Scores" cliqué
                    printf("Bouton Scores clique\n");
                    returning = 2;
                }
                else if (click_in_rect(mouse_x, mouse_y, button_rects[2])) {  // Bouton "Quitter" cliqué
                    printf("Bouton Quitter clique\n");
                    returning = 0;
                    running = 0;
                }
            }
        }
        // Fin gestion des évements

        //Peinture du fond
        SDL_Color renderer_color = {0,0,50,255};
        SDL_SetRenderDrawColor(renderer_menu,renderer_color.r, renderer_color.g, renderer_color.b, renderer_color.a);
        SDL_RenderClear(renderer_menu);

        // Création text "Dooble"
        SDL_Color title_text_color = {255, 255, 255, 255};
        SDL_Surface* tmp_surface = TTF_RenderText_Solid(bigFont, "Dooble", title_text_color);   // Surface "temporaire" pour placer correctement le texte au centre
        SDL_Rect title_rect = {screen.w/2 - tmp_surface->w/2, screen.h/8 - tmp_surface->h/2.5, tmp_surface->w, tmp_surface->h};
        SDL_Rect text_title_rect;
        SDL_FreeSurface(tmp_surface);
        create_button(renderer_menu, title_rect, text_title_rect, "Dooble", bigFont, title_text_color, renderer_color);


        // Création des boutons
        for(int i=0 ; i<3 ; i++) {
            create_button(renderer_menu, button_rects[i], text_button_rects[i], button_names[i], smallFont, button_text_color, button_color);
        }

        // Mis à jour de l'écran
        SDL_RenderPresent(renderer_menu);
    }
    // Fin boucle principale

    if (returning != -1) {
        return returning;
        } else {
        printf("pb avec le returning");
        return -1;
    }
}
