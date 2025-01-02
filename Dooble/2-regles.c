#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"

void maj_gamemode_zone(SDL_Renderer* renderer, SDL_DisplayMode screen, SDL_Rect checkbox[2], SDL_Rect name_input_box, char name_in_progress[50], TTF_Font* medium_font, TTF_Font* small_font, SDL_Color background_color, int multi_activated) {
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};

    // Créaton + maj "Mode de jeu"
    SDL_Rect title_rect = {screen.w/10, screen.h/5, screen.w/5, screen.h/10};
    create_button(renderer, title_rect, "Mode de jeu", medium_font, white_color, background_color);
    // Création + maj "Nom du joueur"
    SDL_Rect title_name_rect = {screen.w/10, screen.h/2.8, screen.w/5, screen.h/10};
    create_button(renderer, title_name_rect, "Nom du joueur", medium_font, white_color, background_color);

    // Création + maj checkbox du mode
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    if(multi_activated) {
        SDL_RenderFillRect(renderer, &checkbox[0]);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderFillRect(renderer, &checkbox[1]);
    } else {
        SDL_RenderFillRect(renderer, &checkbox[1]);
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderFillRect(renderer, &checkbox[0]);
    }
    // Création + maj texte "Solo" et "Multijoueur"
    char* mode_names[2] = {"Solo", "Multijoueur"};
    for(int i=0 ; i<2 ; i++) {
        SDL_Rect text_rect = {checkbox[i].x + screen.w/30 + i*screen.w/35, checkbox[i].y, checkbox[i].w, checkbox[i].h};
        create_button(renderer, text_rect, mode_names[i], small_font, white_color, background_color);
    }
    // Maj visuelle de l'input_box pour la saisie de nom
    maj_input_box(renderer, name_input_box, white_color, name_in_progress, small_font, black_color);
}




void maj_list_player(SDL_Renderer* renderer, SDL_DisplayMode screen, SDL_Rect rects_name[15], SDL_Rect delete_rects[15], char* names_player_save[15], TTF_Font* medium_font, TTF_Font* small_font, SDL_Color background_color, int nb_player, int multi_activated) {
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};
    if(!multi_activated && nb_player>0) nb_player = 1;

    // Creation + maj titre "nb de joueurs : ..."
    SDL_Rect title_name_list = {screen.w/1.2,screen.h/12,screen.w/10,screen.w/50};
    char title[50] = "Nombre de joueurs : ", tmp[5];
    sprintf(tmp, "%d", nb_player);  // Convertit le nb_player en char, puis l'ajoute dans tmp
    strcat(title, tmp);
    create_button(renderer, title_name_list, title, small_font, white_color, background_color);
    // Affichage noms joueurs + croix pour delete en fonction du nb de joueur + nb joueur (J1, J2, ...)
    if(nb_player>0) {
        for(int i=0 ; i<nb_player ; i++) {
            create_button(renderer, rects_name[i], names_player_save[i], small_font, black_color, white_color);
            create_button(renderer, delete_rects[i], "X", small_font, white_color, background_color);
            for(int j=0 ; j<=i ; j++) {
                char nb_player_entre[20] = "J", tmp[5] = "";
                sprintf(tmp, "%d", j+1);
                strcat(nb_player_entre, tmp);
                SDL_Rect tmp_rect = {rects_name[i].x - screen.w/35,  rects_name[i].y,  screen.w/40,  rects_name[i].h};
                create_button(renderer, tmp_rect, nb_player_entre, small_font, white_color, background_color);
            }
            SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            SDL_RenderDrawRect(renderer, &rects_name[i]);
        }
    }
}


void maj_delete_player(int* nb_player_to_delete, int* nb_player, char* names_player_save[15]) {
    if(*nb_player_to_delete != -1) {
        *nb_player = *nb_player - 1;
        for(int i = *nb_player_to_delete ; i<*nb_player ; i++) {
            *names_player_save[i] = *names_player_save[i+1];
        }
        *nb_player_to_delete = -1;
    }
}


void activate_time_input_box(SDL_Event event, int* is_active, char* input_text, char* saved_text, SDL_Rect input_box) {
    if(event.type == SDL_TEXTINPUT && *is_active ==1) {
        strcat(input_text, event.text.text);
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(click_in_rect(input_box)) *is_active = 1;
        else *is_active = 0;
    }
    else if(event.type == SDL_KEYDOWN && *is_active == 1) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_text)>0) {
            input_text[strlen(input_text) - 1] = '\0';
        }
        else if(event.key.keysym.sym == SDLK_RETURN) {
            if(atoi(input_text)>=60 && atoi(input_text)<=180) strcpy(saved_text, input_text);
            input_text[0] = '\0';
        }
    }
}

void activate_words_input_box(SDL_Event event, int* is_active, char* input_text, char* saved_text, SDL_Rect input_box) {
    if(event.type == SDL_TEXTINPUT && *is_active ==1) {
        strcat(input_text, event.text.text);
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(click_in_rect(input_box)) *is_active = 1;
        else *is_active = 0;
    }
    else if(event.type == SDL_KEYDOWN && *is_active == 1) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_text)>0) {
            input_text[strlen(input_text) - 1] = '\0';
        }
        else if(event.key.keysym.sym == SDLK_RETURN) {
            if(atoi(input_text)>=6 && atoi(input_text)<=12) strcpy(saved_text, input_text);
            input_text[0] = '\0';
        }
    }
}








void options_window(SDL_DisplayMode screen, int* nb_window, int* nb_joueurs_at_end, int* time_at_end, int* nb_words_at_end, int* gamemode_at_end, char** names_player_save_at_end) {


    // Création window + renderer + police
    SDL_Window* window_regles = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_regles = SDL_CreateRenderer(window_regles, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* big_font = TTF_OpenFont("../police/arial.ttf", screen.w/15);      // Police grande taille pour titre
    TTF_Font* medium_font = TTF_OpenFont("../police/arial.ttf", screen.w/35);   // Police taille moyenne pour section ("Mode de jeu", "Temps de jeu", ...)
    TTF_Font* small_font = TTF_OpenFont("../police/arial.ttf", screen.w/60);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)
    TTF_Font* very_small_font = TTF_OpenFont("../police/arial.ttf", screen.w/120);    // Police taille petite pour détails ("Solo", "Multi", noms des joueurs dans la liste si multi, ...)


    // Création des structures à modfifier / utiliser dans le main

    SDL_Rect* checkbox_mode = malloc(sizeof(SDL_Rect)*2);
    for(int i=0 ; i<2 ; i++) {
        checkbox_mode[i].w = screen.w/50;
        checkbox_mode[i].h = screen.w/50;
        checkbox_mode[i].x = screen.w/15 + i*checkbox_mode[i].w*8;
        checkbox_mode[i].y = screen.h/3.3;
    }
    SDL_Rect name_input_box = {screen.w/10,screen.h/2.2,screen.w/5,screen.h/15};
    SDL_Rect rule_nb_player_rect = {screen.w/6, screen.h/2.3, screen.w/20, screen.h/150};
    SDL_Rect* rects_name_list = malloc(sizeof(SDL_Rect)*15);
    for(int i=0 ; i<15 ; i++) {
        rects_name_list[i].x = screen.w/1.2;
        rects_name_list[i].y = screen.h/8 + i*screen.w/45;
        rects_name_list[i].w = screen.w/8.5;
        rects_name_list[i].h = screen.w/50;
    }
    SDL_Rect* rect_delete_name_in_list = malloc(sizeof(SDL_Rect)*15);
    for(int i=0 ; i<15 ; i++) {
        rect_delete_name_in_list[i].x = screen.w/1 - screen.w/20;
        rect_delete_name_in_list[i].y = screen.h/8 + i*screen.w/45;
        rect_delete_name_in_list[i].w = screen.w/50;
        rect_delete_name_in_list[i].h = screen.w/50;
    }
    SDL_Rect time_progress_bar = {screen.w/15,screen.h/1.5,screen.w/4,screen.h/15};
    SDL_Rect time_sub_progress_bar;
    SDL_Rect time_input_box = {screen.w/5.5,screen.h/1.3,screen.w/8,screen.h/20};
    SDL_Rect affichage_time = {screen.w/15,screen.h/1.3,screen.w/15,screen.h/20};
    SDL_Rect min_time_bar = {screen.w/15.5,screen.h/1.65,screen.w/20,screen.h/20};
    SDL_Rect max_time_bar = {screen.w/3.5,screen.h/1.65,screen.w/20,screen.h/20};
    SDL_Rect title_time_rect = {screen.w/6, screen.h/1.8, screen.w/20, screen.h/20};

    SDL_Rect words_progress_bar = {screen.w/1.4,screen.h/1.5,screen.w/4,screen.h/15};
    SDL_Rect words_sub_progress_bar;
    SDL_Rect words_input_box = {screen.w/1.2,screen.h/1.3,screen.w/8,screen.h/20};
    SDL_Rect affichage_nb_mots = {screen.w/1.4,screen.h/1.3,screen.w/15,screen.h/20};
    SDL_Rect min_words_bar = {screen.w/1.45,screen.h/1.65,screen.w/20,screen.h/20};
    SDL_Rect max_words_bar = {screen.w/1.1,screen.h/1.65,screen.w/20,screen.h/20};
    SDL_Rect title_nb_words_rect = {screen.w/1.25, screen.h/1.8, screen.w/20, screen.h/20};

    SDL_Rect play_button = {screen.w/4, screen.h/1.2, screen.w/2, screen.h/10};
    SDL_Rect go_back_menu_rect = {0, 0, screen.w/10, screen.h/20};


    // Déclarations des variable utiles dans la boucle
    SDL_Event event;
    int running = 1;
    SDL_Color black_color = {0,0,0,255};
    SDL_Color white_color = {255,255,255,255};

    int nb_player = 0;
    int multi_activated = 0;
    int input_box_enter_name_active = 0;
    int nb_player_to_delete = -1;
    int input_box_time_active = 0;
    int input_box_words_active = 0;
    int time_bar_clicked = 0;
    int int_time_choosen = 60;
    int words_bar_clicked = 0;
    int int_words_choosen = 6;
    float ratio_time = 0;
    float ratio_words = 0;


    char* name_in_progress = malloc(sizeof(char)*50);       name_in_progress[0] = '\0';            // Stocker la saise en cours lors de l'entrée du nom
    // Création d'un tab de 15 pointeurs pouvant contenir chacun 50 caractères
    char* names_player_save[15];
    for(int i=0 ; i<15 ; i++) {
        char* tmp_p = malloc(sizeof(char)*50);
        tmp_p[0] = '\0';
        names_player_save[i] = tmp_p;
    }
    char input_time_select[5] = "";
    char save_time_select[5] = "60";
    char input_words_select[5] = "";
    char save_words_select[5] = "6";


    // Boucle principale
    while(running) {
        //Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
                *nb_window = 0;
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                // Modification mode séléctionné
                if(click_in_rect(checkbox_mode[0])) {
                    multi_activated = 0;
                    for (int i=1 ; i<nb_player ; i++) names_player_save[i][0] = '\0';
                    nb_player = 1;
                }
                if(click_in_rect(checkbox_mode[1])) multi_activated = 1;
                // Supprimer joueur si clic dans la case pour le supprimer
                for(int i=0 ; i<=nb_player ; i++) {
                    if(click_in_rect(rect_delete_name_in_list[i]))  nb_player_to_delete = i;
                }
                // Modification time si clic dans la barre de progression pour le time
                if(click_in_rect(time_progress_bar)) {
                    edit_progress_bar_with_click(time_progress_bar, &time_sub_progress_bar, &ratio_time);
                    time_bar_clicked = 1;
                }
                // Modification time si clic dans la barre de progression pour le nb de mots
                if(click_in_rect(words_progress_bar)) {
                    edit_progress_bar_with_click(words_progress_bar, &words_sub_progress_bar, &ratio_words);
                    words_bar_clicked = 1;
                }
                // Si bouton PLAY cliqué, retourner toutes les infos de jeu et stopper la window
                if(click_in_rect(play_button)) {
                    if(nb_player >0) {
                        running = 0;
                        *nb_joueurs_at_end = nb_player;
                        *time_at_end = int_time_choosen;
                        *nb_words_at_end = int_words_choosen;
                        *gamemode_at_end = multi_activated;
                        for(int i=0 ; i<nb_player ; i++) names_player_save_at_end[i] = names_player_save[i];
                        if(multi_activated) *nb_window = 4;
                        else *nb_window = 3;
                    }
                }
                // Si bouton Menu cliqué, simplement retourner au menu
                if(click_in_rect(go_back_menu_rect)) {
                    running = 0;
                    *nb_window = 1;
                }
            }
            // Activation input_box pour time
            activate_time_input_box(event, &input_box_time_active, input_time_select, save_time_select, time_input_box);
            if(input_box_time_active) time_bar_clicked = 0;
            // Activation input_box pour nb_words
            activate_words_input_box(event, &input_box_words_active, input_words_select, save_words_select, words_input_box);
            if(input_box_words_active) words_bar_clicked = 0;

            // Activation saisie nom joueur + detection si nouveau nom validé ou pas
            if(nb_player<4) {
                char tmp[50];
                strcpy(tmp, names_player_save[nb_player]);
                activate_input_box(event, &input_box_enter_name_active, name_in_progress, names_player_save[nb_player], name_input_box);
                if(strcmp(tmp, names_player_save[nb_player]) != 0) nb_player++;     // Si l'ancien nom saved à été modifié -> nouvelle entrée de nom -> nb_player ++
            }
        }
        // Fin gestion évenements


        // Remplissage fenetre
        SDL_Color background_color = {255,150,100,255};
        SDL_SetRenderDrawColor(renderer_regles, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer_regles);

        // Création titre "Options"
        SDL_Rect title_rect = {screen.w/4, screen.h/15, screen.w/2, screen.h/10};
        create_button(renderer_regles, title_rect, "Options", big_font, white_color, background_color);

        // Maj de la zone de saisie nom + mode de jeu
        maj_gamemode_zone(renderer_regles, screen, checkbox_mode, name_input_box, name_in_progress, medium_font, small_font, background_color, multi_activated);
        create_button(renderer_regles, rule_nb_player_rect, "Rentrez entre 1 et 4 noms de joueurs", very_small_font, white_color, background_color);

        // Maj de la liste des joueurs
        maj_delete_player(&nb_player_to_delete, &nb_player, names_player_save);
        maj_list_player(renderer_regles, screen, rects_name_list, rect_delete_name_in_list, names_player_save, medium_font, small_font, background_color, nb_player, multi_activated);

        // Maj de la zone de choix du temps
        if(!time_bar_clicked) {
            ratio_time = (atof(save_time_select) - 60) / 120;   // Calcul du ratio de la barre à colorier (en sahcant qu"elle va de 60 à 180)
            edit_progress_bar_with_ratio(time_progress_bar, &time_sub_progress_bar, ratio_time*100);
        }
        maj_progress_bar(renderer_regles, time_progress_bar, time_sub_progress_bar, white_color, black_color);
        maj_input_box(renderer_regles, time_input_box, white_color, input_time_select, small_font, black_color);
        char tmp_time[5];
        float tmp1_time = ratio_time*120 +60;
        int_time_choosen = tmp1_time;
        sprintf(tmp_time, "%d", int_time_choosen);
        strcat(tmp_time, " sec");
        create_button(renderer_regles, affichage_time, tmp_time, small_font, black_color, white_color);
        create_button(renderer_regles, min_time_bar, "60 sec", small_font, white_color, background_color);
        create_button(renderer_regles, max_time_bar, "180 sec", small_font, white_color, background_color);
        create_button(renderer_regles, title_time_rect, "Temps de jeu", medium_font, white_color, background_color);

        // Maj de la zone de choix du temps
        if(!words_bar_clicked) {
            ratio_words = (atof(save_words_select) - 6) / 6;
            edit_progress_bar_with_ratio(words_progress_bar, &words_sub_progress_bar, ratio_words*100);
        }
        maj_progress_bar(renderer_regles, words_progress_bar, words_sub_progress_bar, white_color, black_color);
        maj_input_box(renderer_regles, words_input_box, white_color, input_words_select, small_font, black_color);
        char tmp_words[5];
        float tmp1_words = ratio_words*6 + 6;
        int_words_choosen = tmp1_words;
        sprintf(tmp_words, "%d", int_words_choosen);
        strcat(tmp_words, " mots");
        create_button(renderer_regles, affichage_nb_mots, tmp_words, small_font, black_color, white_color);
        create_button(renderer_regles, min_words_bar, "6 mots", small_font, white_color, background_color);
        create_button(renderer_regles, max_words_bar, "12 mots", small_font, white_color, background_color);
        create_button(renderer_regles, title_nb_words_rect, "Nombre de mots en jeu", medium_font, white_color, background_color);

        // Maj bouton Play
        create_button(renderer_regles, play_button, "PLAY", big_font, white_color, background_color);

        // Maj bouton Menu
        create_button(renderer_regles, go_back_menu_rect, "Menu", medium_font, black_color, white_color);

        // Maj visuelle de la fenetre
        SDL_RenderPresent(renderer_regles);
    }
    // Fin boucle

    // Libération mémoire + fermeture SDL
    SDL_DestroyRenderer(renderer_regles);
    SDL_DestroyWindow(window_regles);
    TTF_CloseFont(big_font);
    TTF_CloseFont(medium_font);
    TTF_CloseFont(small_font);

    free(checkbox_mode);
    free(rects_name_list);
    free(rect_delete_name_in_list);
    free(name_in_progress);
}
