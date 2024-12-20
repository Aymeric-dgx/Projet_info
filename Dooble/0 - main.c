#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"

/* nb_window :
0 : Quitter (stopper le programme)
1 : menu
2 : options
3 : game    (ou 3 : solo, 4 : multi ?)
4 : scores
*/

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"


void main(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    // Obtenir les dimensions de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    int general_nb_window = 1;

    int running = 1;

    while (running) {
        // Gestion des événements
        if (general_nb_window == 0) running = 0; // Bouton Quitter cliqué --> stopper le programme
        if(general_nb_window == 1) {
            int nb_window = general_nb_window;
            window_menu(screen, &nb_window);
            general_nb_window = nb_window;
        }
        if (general_nb_window == 2) {
            int nb_window = general_nb_window;
            int nb_player;
            int time;
            int gamemode;
            int nb_words;
            char** names_player_save = malloc(sizeof(char*)*15);
            for(int i=0 ; i<15 ; i++) {
                char* tmp = malloc(sizeof(char)*50);
                tmp[0] = '\0';
                names_player_save[i] = tmp;
            }
            options_window(screen, &nb_window, &nb_player, &time, &nb_words, &gamemode, names_player_save);
            general_nb_window = nb_window;
        }
        if(general_nb_window == 3) running = 0;
    }


    TTF_Quit();
    SDL_Quit();
}
