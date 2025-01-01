#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "option.h"


void set_all_words_rect_pos_for_multi(SDL_Rect big_rect_common_list, SDL_Rect big_rect_current_player_list, SDL_Rect big_rect_all_players, int pos_common_rect[12][4], int pos_current_player_rect[12][4], int pos_all_players_rects[4][12][4]) {
    // Caclul des positons des mots de la liste commune
    for(int i=0 ; i<3 ; i++) {
        for(int j=0 ; j<4 ; j++) {
            pos_common_rect[4*i + j][0] = big_rect_common_list.x + j*big_rect_common_list.w/4;
            pos_common_rect[4*i + j][1] = big_rect_common_list.y + i*big_rect_common_list.h/3;
            pos_common_rect[4*i + j][2] = big_rect_common_list.w/4;
            pos_common_rect[4*i + j][3] = big_rect_common_list.h/3;
        }
    }
    // Caclul des positons des mots de la liste joueur en cours
    for(int i=0 ; i<2 ; i++) {
        for(int j=0 ; j<6 ; j++) {
            pos_current_player_rect[6*i + j][0] = big_rect_current_player_list.x + j*big_rect_current_player_list.w/6;
            pos_current_player_rect[6*i + j][1] = big_rect_current_player_list.y + i*big_rect_current_player_list.h/2;
            pos_current_player_rect[6*i + j][2] = big_rect_current_player_list.w/6;
            pos_current_player_rect[6*i +j][3] = big_rect_current_player_list.h/2;
        }
    }
    // Caclul de la position des mots des listes ed tout les joueurs (en petits)
    for(int i=0 ; i<4 ; i++) {
        for(int j=0 ; j<2 ; j++) {
            for(int k=0 ; k<6 ; k++) {
                pos_all_players_rects[i][6*j + k][0] = big_rect_all_players.x + k*big_rect_all_players.w/6;
                pos_all_players_rects[i][6*j + k][1] = big_rect_all_players.y + i*big_rect_all_players.h/5 + (1+j)*big_rect_all_players.h/15;
                pos_all_players_rects[i][6*j + k][2] = big_rect_all_players.w/6;
                pos_all_players_rects[i][6*j + k][3] = big_rect_all_players.h/20;
            }
        }
    }
}




void window_play_multi(SDL_DisplayMode screen, int nb_player, int nb_words, int play_time, int* scores, char** players_name, int* nb_window) {

    // Création window + renderer + fonts
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* big_font = TTF_OpenFont("../police/arial.ttf", screen.w/50);      // Police grande taille pour titre
    TTF_Font* medium_font = TTF_OpenFont("../police/arial.ttf", screen.w/80);   // Police taille moyenne pour section ("Mode de jeu", "Temps de jeu", ...)
    TTF_Font* small_font = TTF_OpenFont("../police/arial.ttf", screen.w/120);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)
    TTF_Font* very_small_font = TTF_OpenFont("../police/arial.ttf", screen.w/150);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)



    // Création des strucutres
    SDL_Rect big_rect_common_list = {screen.w/2,screen.h/2.5,screen.w/2,screen.h/7};
    SDL_Rect big_rect_current_player_list = {screen.w/8,screen.h/1.3,3*screen.w/4,screen.h/10};
    SDL_Rect big_rect_all_player_list = {0, 0, 2*screen.w/5, 8*screen.h/10};
    SDL_Rect title_common_list_rect = {screen.w-screen.w/4 - screen.w/20, screen.h/2.8, screen.w/10, screen.h/25};
    SDL_Rect title_player_list_rect = {screen.w/2-screen.w/20, screen.h/1.35, screen.w/10, screen.h/50};

    SDL_Rect input_rect = {3*screen.w/8,screen.h/1.1,screen.w/4,screen.h/15};
    SDL_Rect input_title_rect = {screen.w/2-screen.w/20, screen.h/1.125, screen.w/10, screen.h/100};

    SDL_Rect global_time_progress_bar = {screen.w/2, 0, screen.w/2, screen.h/20};
    SDL_Rect global_time_sub_progress_bar;
    SDL_Rect global_time_display = {screen.w-screen.w/6.8, screen.h/18, screen.w/7, screen.h/20};
    SDL_Rect player_time_display = {screen.w - screen.w/6.8,screen.h/1.8,screen.w/7,screen.h/20};

    SDL_Rect score_display = {input_rect.x+input_rect.w, input_rect.y, screen.w/10, screen.h/25};



    // Variables utile dans la boucle

    // Générales
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};
    SDL_Color background_color = {155,100,255,255};
    SDL_Event event;
    int running = 1;

    // Génération des listes des joueurs + commune
    char*** all_player_list = malloc(sizeof(char**)*nb_player);
    for(int i=0 ; i<nb_player ; i++) {
        all_player_list[i] = malloc(sizeof(char*)*nb_words);
        for(int j=0 ; j<nb_words ; j++) {
            all_player_list[i][j] = malloc(sizeof(char)*50);
        }
    }
    char** common_list = malloc(sizeof(char*)*nb_words);
    for(int i=0 ; i<nb_words ; i++) {
        common_list[i] = malloc(sizeof(char)*50);
    }
    generate_list_multi(nb_words, nb_player, all_player_list, common_list, 1, 0);   // Initialisation des listes



    // Positions des mots de chaque listes
    int pos_rect_common_list[12][4];    // [nb rect] [caractéristique : x, y, w, h]
    int pos_current_player_list[12][4];
    int pos_all_player_list[5][12][4];
    set_all_words_rect_pos_for_multi(big_rect_common_list, big_rect_current_player_list, big_rect_all_player_list, pos_rect_common_list, pos_current_player_list, pos_all_player_list);

    // Input box pour la saisie du mot en commun
    char input_text[50] = "";
    char saved_text[50] = "";
    int input_activated = 0;

    // Gestion du temps + tour du joueur
    int player_turn = 0;
    float global_remaining_time_ratio = 1;
    float player_remaining_time_ratio = 1;
    time_t global_time_at_start = time(NULL);
    time_t global_remaining_time = play_time;
    time_t player_time_at_start = time(NULL);
    time_t player_remaining_time = 10;
    char s_time[20];


    // Boucle principale
    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
                *nb_window = 0;
            }
            activate_input_box(event, &input_activated, input_text, saved_text, input_rect);
        }
        // Fin festion des évenements


        // Récupération + calcul des différents temps (à faire en premier)
        time_t current_time = time(NULL);
        global_remaining_time = play_time - (current_time - global_time_at_start);
        player_remaining_time = 10 - (current_time - player_time_at_start);

        // Si le temps du joueur est dépassé, on remet le compteur à 10 sec et on change de joueur
        if(player_remaining_time <=0) {
            player_time_at_start = time(NULL);
            player_turn = (player_turn+1)%nb_player;
        }

        // Si le joueur en cours trouve le mot en commun avec la liste commune : son_score++, reset son chrono, maj des listes (nb : bien vérifié que c'es un mot de sa liste et pas des autres listes)
        int word_found = 0;
        for(int i=0 ; i<nb_words ; i++) {
            for(int j=0 ; j<nb_words ; j++) {
                if(strcmp(saved_text, common_list[i]) == 0 && strcmp(saved_text, all_player_list[player_turn][j]) == 0) {
                    word_found = 1;
                }
            }
        }
        if(word_found) {
            player_time_at_start = time(NULL);
            generate_list_multi(nb_words, nb_player, all_player_list, common_list, 0, player_turn);
            scores[player_turn]++;
        }
        strcpy(saved_text, "");


        // Remplissage du fond de la fenetre
        SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer);

        // Affichage des mots au de la liste commune au centre et de la liste joueur en cours
        for(int i=0 ; i<nb_words ; i++) {
            SDL_Rect tmp_common_rect = {pos_rect_common_list[i][0], pos_rect_common_list[i][1], pos_rect_common_list[i][2], pos_rect_common_list[i][3]};
            SDL_Rect tmp_player_rect = {pos_current_player_list[i][0], pos_current_player_list[i][1], pos_current_player_list[i][2], pos_current_player_list[i][3]};
            create_button(renderer, tmp_common_rect, common_list[i], medium_font, black_color, white_color);
            create_button(renderer, tmp_player_rect, all_player_list[player_turn][i], medium_font, black_color, white_color);
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderDrawRect(renderer, &tmp_player_rect);
            SDL_RenderDrawRect(renderer, &tmp_common_rect);
        }
        create_button(renderer, title_common_list_rect, "Liste commune", big_font, white_color, background_color);
        char tmp_title_char[50];
        sprintf(tmp_title_char, "Liste de %s", players_name[player_turn]);
        create_button(renderer, title_player_list_rect, tmp_title_char, big_font, white_color, background_color);

        // Affichage des liste joueurs, nom, scores ... (en haut à droite en petit)
        for(int i=0 ; i<nb_player; i++) {
            for(int j=0 ; j<nb_words ; j++) {
                SDL_Rect tmp_rect = {pos_all_player_list[i][j][0], pos_all_player_list[i][j][1], pos_all_player_list[i][j][2], pos_all_player_list[i][j][3]};
                create_button(renderer, tmp_rect, all_player_list[i][j], very_small_font, black_color, background_color);
            }
            SDL_Rect tmp_title_rect = {0, pos_all_player_list[i][0][1]-screen.h/25, screen.w/10, screen.h/25};
            char tmp_title_text[30];
            sprintf(tmp_title_text, "%s : %d point(s)", players_name[i], scores[i]);
            create_button(renderer, tmp_title_rect, tmp_title_text, small_font, black_color, white_color);
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_Rect tmp_small_rect = {pos_all_player_list[i][0][0], pos_all_player_list[i][0][1], big_rect_all_player_list.w, big_rect_all_player_list.h/8};
            SDL_RenderDrawRect(renderer, &tmp_small_rect);
        }


        // Maj de la progress_bar correspondant au joueur en cours
        float f_player_remaining_time = player_remaining_time;
        player_remaining_time_ratio = f_player_remaining_time / 10;
        SDL_Rect current_player_progress_bar = {screen.w/8, pos_all_player_list[player_turn][0][1]-screen.h/30, screen.w/10, screen.h/45};
        SDL_Rect current_sub_player_progress_bar;
        edit_progress_bar_with_ratio(current_player_progress_bar, &current_sub_player_progress_bar, player_remaining_time_ratio);
        maj_progress_bar(renderer, current_player_progress_bar, current_sub_player_progress_bar, white_color, black_color);


        // Maj de la progres_bar du global_time + affichage du temps
        float f_global_remaining_time = global_remaining_time, f_play_time = play_time;
        global_remaining_time_ratio = f_global_remaining_time / f_play_time;
        edit_progress_bar_with_ratio(global_time_progress_bar, &global_time_sub_progress_bar, global_remaining_time_ratio);
        maj_progress_bar(renderer, global_time_progress_bar, global_time_sub_progress_bar, white_color, black_color);

        sprintf(s_time, "Temps de jeu : %lld sec", global_remaining_time);
        create_button(renderer, global_time_display, s_time, medium_font, black_color, white_color);
        sprintf(s_time, "Temps du tour : %lld sec", player_remaining_time);
        create_button(renderer, player_time_display, s_time, medium_font, black_color, white_color);


        // Maj visuelle de l'input_box
        maj_input_box(renderer, input_rect, white_color, input_text, medium_font, black_color);
        create_button(renderer, input_title_rect, "Entrez le mot en commun ci-dessous (en majuscules)", small_font, white_color, background_color);


        // Si le temps de jeu est dépassé, afficher une image de fin
        if(global_remaining_time<=0) {
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderClear(renderer);
            char final_text[50] = "Jeu termine, felicitations ! Voici vos scores";
            SDL_Rect tmp_rect = {screen.w/2-screen.w/6, screen.h/25, screen.w/3, screen.h/20};
            create_button(renderer, tmp_rect, final_text, big_font, black_color, white_color);
            for(int i=0 ; i<nb_player ; i++) {
                SDL_Rect tmp_rect2 = {screen.w/4, screen.h/10 + i*screen.h/15, screen.w/2, screen.h/10};
                sprintf(final_text, "- %s : %d point(s)", players_name[i], scores[i]);
                create_button(renderer, tmp_rect2, final_text, big_font, black_color, white_color);
            }
        }

        // Après un certain temps, arreter le programme
        if(global_remaining_time <= -5) {
            running = 0;
            *nb_window = 1;
        }


        // Maj de la fenetre
        SDL_RenderPresent(renderer);

    }
    // Fin boucle principale

    // Libération de la mémoire
    for(int i=0 ; i<nb_player ; i++) {
        for(int j=0 ; j<nb_words ; j++) {
             free(all_player_list[i][j]);
        }
        free(all_player_list[i]);
    }
    free(all_player_list);

    for(int i=0 ; i<nb_words ; i++) {
        free(common_list[i]);
    }
    free(common_list);

    // Destruction propre
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(big_font);
    TTF_CloseFont(medium_font);
    TTF_CloseFont(small_font);
    TTF_CloseFont(very_small_font);
}
