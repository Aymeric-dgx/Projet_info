#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"


void set_all_words_rect_pos(SDL_Rect big_rect_common_list, SDL_Rect big_rect_player_list, int pos_common_rect[12][4], int pos_player_rect[12][4]) {
    // Caclul des positons des mots de la liste commune
    for(int i=0 ; i<3 ; i++) {
        for(int j=0 ; j<4 ; j++) {
            pos_common_rect[4*i + j][0] = big_rect_common_list.x + j*big_rect_common_list.w/4;
            pos_common_rect[4*i + j][1] = big_rect_common_list.y + i*big_rect_common_list.h/3;
            pos_common_rect[4*i + j][2] = big_rect_common_list.w/4;
            pos_common_rect[4*i + j][3] = big_rect_common_list.h/3;
        }
    }
    // Caclul des positons des mots de la liste joueur
    for(int i=0 ; i<2 ; i++) {
        for(int j=0 ; j<6 ; j++) {
            pos_player_rect[6*i + j][0] = big_rect_player_list.x + j*big_rect_player_list.w/6;
            pos_player_rect[6*i + j][1] = big_rect_player_list.y + i*big_rect_player_list.h/2;
            pos_player_rect[6*i + j][2] = big_rect_player_list.w/6;
            pos_player_rect[6*i +j][3] = big_rect_player_list.h/2;
        }
    }
}





int main() {
    // Variables à récupérer en dehors de la fonction
    int nb_words = 7;
    int time = 165;
    char* name_player = "Jack";

    // Initialisation SDL + récupération dimensions écran
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    // Création window + renderer + fonts
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* big_font = TTF_OpenFont("../police/arial.ttf", screen.w/15);      // Police grande taille pour titre
    TTF_Font* medium_font = TTF_OpenFont("../police/arial.ttf", screen.w/35);   // Police taille moyenne pour section ("Mode de jeu", "Temps de jeu", ...)
    TTF_Font* small_font = TTF_OpenFont("../police/arial.ttf", screen.w/100);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)


    // Création des strucutres
    SDL_Rect big_rect_common_list = {screen.w/4,screen.h/3,screen.w/2,screen.h/7};
    SDL_Rect bigplayer_words_rect = {screen.w/6,screen.h/1.4,2*screen.w/3,screen.h/10};
    SDL_Rect input_rect = {3*screen.w/8,screen.h/1.15,screen.w/4,screen.h/15};



    // Variables utile dans la boucle

    // Générales
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};
    SDL_Color background_color = {155,100,255,255};
    SDL_Event event;
    int running = 1;

    // Positions des mots de chaque listes
    int pos_rect_common_list[12][4];    // [nb rect] [caractéristique : x, y, w, h]
    int pos_player_list[12][4];
    set_all_words_rect_pos(big_rect_common_list, bigplayer_words_rect, pos_rect_common_list, pos_player_list);

    // Création + génération des listes (commune et joueur)
    char** player_list = malloc(sizeof(char*)*nb_words);
    char** common_list = malloc(sizeof(char*)*nb_words);
    char* common_word = malloc(sizeof(char)*50);
    for(int i=0 ; i<nb_words ; i++) {
        player_list[i] = malloc(sizeof(char)*50);
        common_list[i] = malloc(sizeof(char)*50);
        player_list[i][0] = '\0';
        common_list[i][0] = '\0';
    }
    printf("hello");
    generate_list_solo(nb_words, player_list, common_list, common_word, 1);

    // Input box pour la saisie du mot en commun
    char input_text[50] = "";
    char saved_text[50] = "";
    int input_activated = 0;


    // Boucle principale
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            activate_input_box(event, &input_activated, input_text, saved_text, input_rect);    // Activation de l'input_box pour la saisie du mot
        }
        // Fin gestion des évenements


        // Si le mots est trouvé, on remplace la liste commune par celle du joueur, et on génère une nouvelle liste joueur
        if(strcmp(common_word, saved_text) == 0) {
            generate_list_solo(nb_words, player_list, common_list, common_word, 0);
        }

        // Remplissage du fond de la fenetre
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer);

        // Affichage des mots de la liste commune et de la liste joueur
        for(int i=0 ; i<nb_words ; i++) {
            SDL_Rect tmp_common_rect = {pos_rect_common_list[i][0], pos_rect_common_list[i][1], pos_rect_common_list[i][2], pos_rect_common_list[i][3]};
            SDL_Rect tmp_player_rect = {pos_player_list[i][0], pos_player_list[i][1], pos_player_list[i][2], pos_player_list[i][3]};
            create_button(renderer, tmp_common_rect, common_list[i], small_font, white_color, background_color);
            create_button(renderer, tmp_player_rect, player_list[i], small_font, white_color, background_color);
            // Dessin des contours en blanc
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderDrawRect(renderer, &tmp_player_rect);
            SDL_RenderDrawRect(renderer, &tmp_common_rect);
        }

        // Maj visuelle de l'input_box
        maj_input_box(renderer, input_rect, white_color, input_text, small_font, black_color);


        // Maj visuelle de la fenetre
        SDL_RenderPresent(renderer);
    }
    // Fin boucle prinicpale

    // Libération de la mémoire + destrcution SDL etc
    for(int i=0 ; i<nb_words ; i++) {
        free(player_list[i]);
        free(common_list[i]);
    }
    free(player_list);
    free(common_list);
    free(common_word);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_Quit();
}
