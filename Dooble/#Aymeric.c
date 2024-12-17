#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"

/* Gestion fenetre regles, cahier des charges :
nb mots = entre 6 et 12
temps de jeu = entre 60 et 120 sec
nb de joueurs max = ?

Visuel :
    - En haut, titre "Options"

    - Partie gauche, de haut en bas :
        - Mode de jeu : check box a colorier en fonction du mode choisie (mode par défaut : solo)
        - Nom joueur (input_box) :
            - Si solo, laisser afficher le nom après entrée
            - Si multi, save nom dans liste (voir ci-dessous) et effacer le texte dans l'input box pour préparer une new saisie
        - Temps de jeu : progress_bar + input_box + affichage tps sélectionné

    - Partie droite, de bas en haut :
        - Si multi :
            - Centre haut : choix nb joueurs (progress_bar + input_box + affichage tps sélectionné)
            - Droite haut : Tableau de la taille du nb joueurs choisi qui affiche les noms sélectioné pour l'instant
        - En face de choix temps (droite/centre bas) : choix nb mots (progress_bar + input_box + affichage tps sélectionné)


Technique :
    Retourner grâce aux pointeurs APRES CLIC SUR PLAY dans main (pour réutiliser dans d'autres fenetres) :
        - Mode de jeu : 0 pour solo, 1 pour multi
        - Nb de joueurs : tab de char avec tout les noms, meme si solo (dans ce cas la, on affectera a une variable l'unique nom pour ne pas se trainer un tableau ?)
        - Nb de mots : int
        - Temps de jeu : int
*/

void maj_zone_gamemode(SDL_Renderer* renderer_regles, SDL_DisplayMode screen, TTF_Font* medium_font, TTF_Font* small_font, SDL_Color background_color, SDL_Rect* checkbox, SDL_Rect* input_name_box, int multi_activated) {
    SDL_Color white_color = {255,255,255,255};

    // Création texte "Mode de jeu"
    SDL_Rect mode_rect = {screen.w/10, screen.h/5, screen.w/5, screen.h/10};
    create_button(renderer_regles, mode_rect, "Mode de jeu", medium_font, white_color, background_color);

    // Remplissage checkbox
    SDL_SetRenderDrawColor(renderer_regles, 255,255,255,255);
    SDL_RenderFillRect(renderer_regles, &checkbox[0]);
    SDL_RenderFillRect(renderer_regles, &checkbox[1]);

    // Coloriage en noir de la checkbox du mode de jeu choisie
    SDL_SetRenderDrawColor(renderer_regles, 0,0,0,255);
    if(multi_activated) SDL_RenderFillRect(renderer_regles, &checkbox[1]);
    else SDL_RenderFillRect(renderer_regles, &checkbox[0]);

    // Création texte mode (Solo et Multijoueurs)
    char* mode_names[2] = {"Solo", "Multijoueurs"};
    for (int i=0 ; i<2 ; i++) {
        SDL_Rect text_rect = {  checkbox[i].x + screen.w/30 + i*screen.w/35,    //  i*screen.w/35 car Multijoueurs trop long -> néscessaire de le décaler un peu
                                checkbox[i].y,
                                checkbox[i].w,
                                checkbox[i].h};

        create_button(renderer_regles, text_rect, mode_names[i], small_font, white_color, background_color);
    }
    input_name_box->x = screen.w/10;
    input_name_box->y = screen.h/2.5;
    input_name_box->w = screen.w/5;
    input_name_box->h = screen.h/15;
}


void maj_liste_joueur(SDL_Renderer* renderer_regles, SDL_DisplayMode screen, SDL_Rect* box_list_name[15], int* nb_player, int multi_activated, char* names_player_save[15], TTF_Font* small_font) {
    if(!multi_activated && *nb_player>0) *nb_player = 1;
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};
    for(int i=0 ; i<*nb_player ; i++) {
        box_list_name[i]->x = screen.w/1.2;
        box_list_name[i]->y = screen.h/8 + i*screen.w/50;
        box_list_name[i]->w = screen.w/10;
        box_list_name[i]->h = screen.w/50;
        create_button(renderer_regles, *box_list_name[i], names_player_save[i], small_font, black_color, white_color);
        SDL_SetRenderDrawColor(renderer_regles, 0,0,0,255);
        SDL_RenderDrawRect(renderer_regles, box_list_name[i]);
    }
}





int main() {
    // Initialisation + obtention dimsnesion écran (à supprimer une fois intégré au projets, en ajoutant en parametre une strcuture scrren)
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

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



    // Création checkbox de choisie de mode de jeu (checkbox n°0 = solo et n°1 = multi)
    SDL_Rect checkbox[2];
    for(int i=0 ; i<2 ; i++) {
        checkbox[i].w = screen.w/50;
        checkbox[i].h = screen.w/50;
        checkbox[i].x = screen.w/15 + i*checkbox[i].w*8;
        checkbox[i].y = screen.h/3;
    }
    // Création des rectangles de la listes des noms, pour pouvoir vérifier si on a cliqué ou pas sur un nom pour le modifier / supprimer
    SDL_Rect* box_list_name = malloc(sizeof(SDL_Rect)*15);



    // Déclaration variable à utiliser dans la boucle principale
    SDL_Event event;
    int running = 1;
    SDL_Color white_color = {255,255,255,255};
    SDL_Color black_color = {0,0,0,255};

    // Variable pour la saisie nom
    SDL_Rect input_name_box = {0,0,0,0};
    int input_name_box_active = 0;
    char* player_name_in_progress = malloc(sizeof(char)*50);    player_name_in_progress[0] = '\0';
    char* player_name_saved = malloc(sizeof(char)*50);          player_name_saved[0] = '\0';
    int multi_activated = 0;
    int nb_player = 0;
    // Création d'un tab de 15 pointeurs pouvant contenir chacun 50 caractères
    char* names_player_save[15];
    for(int i=0 ; i<15 ; i++) {
        char* tmp_p = malloc(sizeof(char)*50);
        tmp_p[0] = '\0';
        names_player_save[i] = tmp_p;
    }


    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            // Début gestion entrée + sauvegarde des noms
            char tmp[50];
            strcpy(tmp, names_player_save[nb_player]);
            activate_input_box(event, &input_name_box_active, player_name_in_progress, names_player_save[nb_player], input_name_box);
            if (strcmp(tmp, names_player_save[nb_player]) != 0) nb_player++;
            // FIn gestion entrée + sauvegarde des noms
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(click_in_rect(checkbox[0])) multi_activated = 0;
                if(click_in_rect(checkbox[1])) multi_activated = 1;
            }

        }
        // Remplissage fenetre
        SDL_Color background_color = {255,150,100,255};
        SDL_SetRenderDrawColor(renderer_regles, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer_regles);

        // Création titre "Options"
        SDL_Rect title_rect = {screen.w/4, screen.h/15, screen.w/2, screen.h/10};
        SDL_Color title_font_color = {255,255,255,255};
        create_button(renderer_regles, title_rect, "Options", big_font, title_font_color, background_color);

        // Gestion graphique zone mode de jeu + entrée nom + liste joueurs
        maj_zone_gamemode(renderer_regles, screen, medium_font, small_font, background_color, checkbox, &input_name_box, multi_activated);
        maj_input_box(renderer_regles, input_name_box, white_color, player_name_in_progress, small_font, black_color);
        maj_liste_joueur(renderer_regles, screen, &box_list_name, &nb_player, multi_activated, names_player_save, small_font);
        SDL_Rect nb_player_button = {screen.w/1.2,screen.h/12,screen.w/10,screen.w/50};
        char affichage_nb_player[50] = "Nombre de joueurs : ", tmp[50] = " salut";
        sprintf(tmp, "%d", nb_player);
        strcat(affichage_nb_player, tmp);
        create_button(renderer_regles, nb_player_button, affichage_nb_player, small_font, white_color, background_color);

        // Maj visuel
        SDL_RenderPresent(renderer_regles);
    }
}
