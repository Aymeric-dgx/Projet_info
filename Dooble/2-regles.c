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


    // Initialisation box de sasie pour nom de joueur
    SDL_Rect name_input_box = {0,0,0,0};

    // Création checkbox de choisie de mode de jeu (checkbox n°0 = solo et n°1 = multi)
    SDL_Rect checkbox[2];
    for(int i=0 ; i<2 ; i++) {
        checkbox[i].w = screen.w/50;
        checkbox[i].h = screen.w/50;
        checkbox[i].x = screen.w/15 + i*checkbox[i].w*8;
        checkbox[i].y = screen.h/3;
    }


    //Déclaration variable que l'on va utiliser dans la boucle principale
    SDL_Event event;
    int running = 1;

    int mode_input_box_active = 0;  // Pour savoir si la box de saisie des noms est activée ou pas

    char* player_name_in_progress = malloc(sizeof(char)*50);    // Pour sauvegarder la saisie en cours
    player_name_in_progress[0] = '\0';

    // Création d'un tab de 15 pointeurs pouvant contenir chacun 50 caractères
    char* player_names_saved[15];
    for(int i=0 ; i<15 ; i++) {
        char* tmp_p = malloc(sizeof(char)*50);
        tmp_p[0] = '\0';
        player_names_saved[i] = tmp_p;
    }

    int multi_activated = 0;    // 0 : solo   1 : multijoueurs
    int nb_player = 0;  // Pour savoir quel joueur est en train de rentrer son nom (surtout utile pour multi)


    // Début boucle principale
    while(running) {
        // Début gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            // Début saisie nom joueur
            char tmp1[50] = "";
            strcpy(tmp1, player_names_saved[nb_player]);
            activate_input_box(event, &mode_input_box_active, player_name_in_progress, player_names_saved[nb_player], name_input_box);
            int new_name_saved = strcmp(tmp1, player_names_saved[nb_player]); // strcmp renvoie 1 OU -1 si les str sont !=, et non pas 1 ou 0 (ce qui aurait été + simple ici)
            if (new_name_saved != 0) nb_player++;   // Si un nouveau nom a été sauvegardé, nb_joueur s'incrémente de 1
            // Fin saisie nom joueur
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                // Clic dans les checkbox de choisie de mode
                if(click_in_rect(x, y, checkbox[0])) multi_activated = 0; // Mode sélectionné : Solo
                if(click_in_rect(x, y, checkbox[1])) multi_activated = 1;// Mode sélectionné : Multi
            }
        }

        // Fin gestion des évenements

        // Remplissage fenetre
        SDL_Color background_color = {255,150,100,255};
        SDL_SetRenderDrawColor(renderer_regles, background_color.r, background_color.g, background_color.b, background_color.a);
        SDL_RenderClear(renderer_regles);

        // Création titre "Options"
        SDL_Rect title_rect = {screen.w/4, screen.h/15, screen.w/2, screen.h/10};
        SDL_Color title_font_color = {255,255,255,255};
        create_button(renderer_regles, title_rect, "Options", big_font, title_font_color, background_color);




        // Gestion zone choix mode de jeu

        // Création texte "Mode de jeu"
        SDL_Rect mode_rect = {screen.w/10, screen.h/5, screen.w/5, screen.h/10};
        create_button(renderer_regles, mode_rect, "Mode de jeu", medium_font, title_font_color, background_color);
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

            create_button(renderer_regles, text_rect, mode_names[i], small_font, title_font_color, background_color);
        }
        // Maj visuelle de mode_input_box
        SDL_Color name_font_color = {0,0,0,255};
        name_input_box.x = screen.w/10;
        name_input_box.y = screen.h/2.5;
        name_input_box.w = screen.w/5;
        name_input_box.h = screen.h/15;
        maj_input_box(renderer_regles, name_input_box, title_font_color, player_name_in_progress, small_font, name_font_color);




        // Maj visuelle de la fenetre
        SDL_RenderPresent(renderer_regles);
    }
    printf("%d",nb_player);
}
