#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
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





void window_play_solo(SDL_DisplayMode screen, int* nb_window, int nb_words, int play_time, int* score) {

    // Création window + renderer + fonts
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* big_font = TTF_OpenFont("../police/arial.ttf", screen.w/15);      // Police grande taille pour titre
    TTF_Font* medium_font = TTF_OpenFont("../police/arial.ttf", screen.w/35);   // Police taille moyenne pour section ("Mode de jeu", "Temps de jeu", ...)
    TTF_Font* small_font = TTF_OpenFont("../police/arial.ttf", screen.w/60);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)
    TTF_Font* very_small_font = TTF_OpenFont("../police/arial.ttf", screen.w/100);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)


    // Création des strucutres
    SDL_Rect big_rect_common_list = {screen.w/4,screen.h/3,screen.w/2,screen.h/7};
    SDL_Rect big_rect_player_list = {screen.w/8,screen.h/1.4,3*screen.w/4,screen.h/10};
    SDL_Rect title_common_list_rect = {screen.w/2-screen.w/20, screen.h/3.5, screen.w/10, screen.h/25};
    SDL_Rect title_player_list_rect = {screen.w/2-screen.w/20, screen.h/1.5, screen.w/10, screen.h/25};

    SDL_Rect input_rect = {3*screen.w/8,screen.h/1.1,screen.w/4,screen.h/15};
    SDL_Rect input_title_rect = {screen.w/2-screen.w/20, screen.h/1.15, screen.w/10, screen.h/25};

    SDL_Rect time_progress_bar = {0, 0, screen.w, screen.h/10};
    SDL_Rect time_sub_progress_bar;
    SDL_Rect time_display = {screen.w/2-screen.w/20, screen.h/9, screen.w/10, screen.h/15};

    SDL_Rect score_display = {input_rect.x+input_rect.w, input_rect.y, screen.w/10, screen.h/25};



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
    set_all_words_rect_pos(big_rect_common_list, big_rect_player_list, pos_rect_common_list, pos_player_list);

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
    generate_list_solo(nb_words, player_list, common_list, common_word, 1); // 1 pour préciser que c'est la 1ère initiliastion (ensuite on mettra uniquement 0)

    // Input box pour la saisie du mot en commun
    char input_text[50] = "";
    char saved_text[50] = "";
    int input_activated = 0;

    // Gestion du temps
    time_t time_at_start = time(NULL);
    time_t remaining_time = play_time;
    float remaining_time_ratio = 1;
    char s_time[10];

    // Gestion du score
    char s_score[5];


    // Boucle principale
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
                *nb_window = 0;
            }
            activate_input_box(event, &input_activated, input_text, saved_text, input_rect);    // Activation de l'input_box pour la saisie du mot
        }
        // Fin gestion des évenements


        // Remplissage du fond de la fenetre
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer);

        // Si le mots est trouvé : score++ et on génère les nouvelles liste (selons les consignes)
        if(strcmp(common_word, saved_text) == 0) {
            generate_list_solo(nb_words, player_list, common_list, common_word, 0);
            (*score)++;
            strcpy(saved_text, "");
            printf("%s ", common_word);
        }

        // Calcul + affichage temps restant
        time_t current_time = time(NULL);
        remaining_time = play_time - (current_time - time_at_start);
        float f_remaining_time = remaining_time, f_play_time = play_time;
        remaining_time_ratio = f_remaining_time / f_play_time;

        edit_progress_bar_with_ratio(time_progress_bar, &time_sub_progress_bar, remaining_time_ratio);
        maj_progress_bar(renderer, time_progress_bar, time_sub_progress_bar, white_color, black_color);
        sprintf(s_time, "%lld sec", remaining_time);
        create_button(renderer, time_display, s_time, small_font, black_color, white_color);


        // Affichage des mots de la liste commune et de la liste joueur
        for(int i=0 ; i<nb_words ; i++) {
            SDL_Rect tmp_common_rect = {pos_rect_common_list[i][0], pos_rect_common_list[i][1], pos_rect_common_list[i][2], pos_rect_common_list[i][3]};
            SDL_Rect tmp_player_rect = {pos_player_list[i][0], pos_player_list[i][1], pos_player_list[i][2], pos_player_list[i][3]};
            create_button(renderer, tmp_common_rect, common_list[i], very_small_font, white_color, background_color);
            create_button(renderer, tmp_player_rect, player_list[i], very_small_font, white_color, background_color);
            // Dessin des contours en blanc
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderDrawRect(renderer, &tmp_player_rect);
            SDL_RenderDrawRect(renderer, &tmp_common_rect);
        }
        create_button(renderer, title_common_list_rect, "Liste commune", small_font, white_color, background_color);
        create_button(renderer, title_player_list_rect, "Liste joueur", small_font, white_color, background_color);

        // Maj visuelle de l'input_box
        maj_input_box(renderer, input_rect, white_color, input_text, small_font, black_color);
        create_button(renderer, input_title_rect, "Entrez le mot en commun ci-dessous (en majuscules)", very_small_font, white_color, background_color);

        // Affichage du score à coté de l'input_box
        sprintf(s_score, "%d point(s)", *score);
        create_button(renderer, score_display, s_score, small_font, white_color, background_color);

        // Si temps écoulé, montrer une image de fin, et après un certain temps arrêter le programme
        if(remaining_time <= 0) {
            char final_text[100];
            sprintf(final_text, "Votre score : %d points", *score);
            SDL_Rect final_rect1 = {screen.w/2-screen.w/10,screen.h/3,screen.w/10,screen.w/25};
            SDL_Rect final_rect2 = {screen.w/2-screen.w/10,2*screen.h/3,screen.w/10,screen.w/25};
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderClear(renderer);
            create_button(renderer, final_rect1, "Jeu termine, felicitations !!!", big_font, black_color, white_color);
            create_button(renderer, final_rect2, final_text, big_font, black_color, white_color);
            if(remaining_time == -5) running = 0;
            *nb_window = 1;
        }


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
    TTF_CloseFont(big_font);
    TTF_CloseFont(medium_font);
    TTF_CloseFont(small_font);
    TTF_CloseFont(very_small_font);
}
