#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "option.h"

#define MAX_SCORE 100
#define MAX_LENGTH 256

void read_score(int *nbScores, char ***scores, char ***joueurs) {
    FILE *file;
    char line[MAX_LENGTH];
    *scores = malloc(MAX_SCORE * sizeof(char*));
    *joueurs = malloc(MAX_SCORE * sizeof(char*));
    int i = 0;

    file = fopen("../save.txt", "r");

    // Vérification de l'ouverture du fichier
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file) && i < MAX_SCORE) {
        line[strcspn(line, "\n")] = '\0'; // Supprime le caractère \n
        int longueur = strlen(line);
        line[longueur - 3] = '\0'; // Sépare les scores et les joueurs

        (*joueurs)[i] = malloc(strlen(line) + 1);
        strcpy((*joueurs)[i], line);

        // Correction de l'allocation de mémoire pour les scores
        (*scores)[i] = malloc(3); // Allouer 3 caractères pour le score (2 chiffres + '\0')
        strcpy((*scores)[i], &line[longueur - 2]);

        i++;
    }

    for (int j = 1; j < i; j++) {
        // Utilisation de la fonction atoi qui transforme un str en int
        while (j > 0 && atoi((*scores)[j - 1]) < atoi((*scores)[j])) {
            // Échange des scores
            char *temp_score = (*scores)[j];
            (*scores)[j] = (*scores)[j - 1];
            (*scores)[j - 1] = temp_score;

            // Échange des joueurs correspondants
            char *temp_joueur = (*joueurs)[j];
            (*joueurs)[j] = (*joueurs)[j - 1];
            (*joueurs)[j - 1] = temp_joueur;
            j--;
        }
    }
    fclose(file);
    *nbScores = i;
}

// La fonction recherche devrait accepter nbScores et **joueurs**
void recherche(int nbScores, char **joueurs,char **scores, const char *pseudo, char ***joueurs_rechercher, char ***scores_j_rechercher, int *nb_j_rechercher) {
    // Compte le nombre de pseudos trouvés
    int count = 0;
    for (int i = 0; i < nbScores; i++) {
        if (strcmp(joueurs[i], pseudo) == 0) {
            // Ajoute les résultats à **joueurs_rechercher et **scores_j_rechercher**
            (*joueurs_rechercher)[count] = joueurs[i];
            (*scores_j_rechercher)[count] = scores[i];
            count++;
        }
    }
    *nb_j_rechercher = count;
}

void window_scores(SDL_DisplayMode screen, int* nb_window) {

    //creation globale de la fenêtre
    SDL_Window* window = SDL_CreateWindow("Dooble",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen.w, screen.h,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", screen.w/28.5);

    SDL_Rect text_box = {screen.w / 2 - screen.w/5, screen.h/10, screen.w/2.5, screen.h/12};
    SDL_Rect go_back_menu_rect = {0, 0, screen.w/6, screen.h/10};
    SDL_Rect reset_rect = {3*screen.w / 4, screen.h/10, screen.w/6, screen.h/12};
    SDL_Rect bar = {3*screen.w / 3.75, screen.h/4, screen.w/6, screen.h/15};
    SDL_Rect ratio_text = {3*screen.w / 3.75, screen.h/3.75, 20, 25};
    SDL_Rect sub_bar = {3*screen.w / 3.75, screen.h/4, screen.w/6, screen.h/15};
    SDL_Rect* p_sub_bar = &sub_bar;

    SDL_Color font_color_r = {0, 0, 0, 255};
    SDL_Color button_rect_color = {255, 255, 255, 255};

    SDL_Color box_color = {255, 255, 255, 255};
    SDL_Color font_color = {0, 0, 0, 255};

    SDL_Color bar_color = {255,255,255,255};
    SDL_Color sub_bar_color = {0,0,0,255};





    SDL_Event event;
    float ratio=1;
    int nbScores;
    char** scores = NULL;
    char** joueurs = NULL;
    char** joueurs_rechercher = malloc(MAX_SCORE * sizeof(char*));
    char** scores_j_rechercher = malloc(MAX_SCORE * sizeof(char*));
    int running = 1;
    int is_active = 0;


    char* input_text = malloc(sizeof(char) * 50);
    input_text[0] = '\0';

    char* saved_text = malloc(sizeof(char) * 50);
    saved_text[0] = '\0';


    read_score(&nbScores, &scores, &joueurs);

    while (running) {
        const char *pseudo_recherche = saved_text;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                    *nb_window = 0;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if(click_in_rect(go_back_menu_rect)) {
                    running = 0;
                    *nb_window = 1;
                }
                if(click_in_rect(reset_rect)) {
                    saved_text[0] = "\0";
                }
                if(click_in_rect(bar)) {
                    edit_progress_bar_with_click(bar, p_sub_bar, &ratio);
                }
            }

            activate_input_box(event, &is_active, input_text, saved_text, text_box);
        }
        SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);

        SDL_RenderClear(renderer);







        maj_input_box(renderer, text_box, box_color, input_text, font, font_color);

        int grille = ratio*8;


        int nb_j_rechercher;
        recherche(nbScores, joueurs,scores, pseudo_recherche, &joueurs_rechercher, &scores_j_rechercher, &nb_j_rechercher);
        if (nb_j_rechercher == 0) {
            if (scores != NULL && joueurs != NULL) {
                for (int j = 0; j < grille; j++) {
                    // création de la première colonne avec les pseudos
                    SDL_Rect button_rect1 = {screen.w / 2 + 2.5, screen.h/4 + j * (int)round(screen.h/10) + j , screen.w/3.5, screen.h/10};
                    create_button(renderer, button_rect1, scores[j], font, font_color_r, button_rect_color);

                    // création de la seconde colonne avec les scores
                    SDL_Rect button_rect2 = {screen.w/2 - screen.w/3.5 + 2.5, screen.h/4 + j * (int)round(screen.h/10) + j , screen.w/3.5, screen.h/10};
                    create_button(renderer, button_rect2, joueurs[j], font, font_color_r, button_rect_color);
                }
            }
        } else {
            if (nb_j_rechercher<grille) grille = nb_j_rechercher;
            if (scores != NULL && joueurs != NULL) {
                for (int k = 0; k < grille; k++) {

                    // création de la première colonne avec les pseudos
                    SDL_Rect button_rect1 = {screen.w / 2 + 2.5, screen.h/4 + k * (int)round(screen.h/10) + k , screen.w/3.5, screen.h/10};
                    create_button(renderer, button_rect1, scores_j_rechercher[k], font, font_color_r, button_rect_color);

                    // création de la seconde colonne avec les scores
                    SDL_Rect button_rect2 = {screen.w/2 - screen.w/3.5 + 2.5, screen.h/4 + k * (int)round(screen.h/10) + k , screen.w/3.5, screen.h/10};
                    create_button(renderer, button_rect2, joueurs_rechercher[k], font, font_color_r, button_rect_color);
                }
            }
        }
        create_button(renderer, go_back_menu_rect, "Menu", font, font_color_r, button_rect_color);
        create_button(renderer,reset_rect, "Reset", font, font_color_r, button_rect_color);

        // Si on veut remplir la barre de progression à 82% :
        //edit_progress_bar_with_ratio(bar, p_sub_bar, 82);
        maj_progress_bar(renderer, bar, sub_bar, bar_color, sub_bar_color);
        SDL_RenderPresent(renderer);
    }

    // Libération de la mémoire
    free(input_text);
    free(saved_text);
    free(joueurs_rechercher);
    free(scores_j_rechercher);

    if (scores != NULL) {
        for (int i = 0; i < nbScores; i++) {
            free(scores[i]);
        }
        free(scores);
    }

    if (joueurs != NULL) {
        for (int i = 0; i < nbScores; i++) {
            free(joueurs[i]);
        }
        free(joueurs);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
