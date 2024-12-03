#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"

/* Aymeric : renommer les fichiers
main
1-menu
2-start
pour qu il corresponde au "numero" de leur fenetre respective, pour que ce soit moins ambigue ?
*/


int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    // Obtenir les dimensions de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_width = screen.w;
    int screen_height = screen.h;


    int running = 1;
    SDL_Event event;    // On en pas besoin pour l'instant


    int nb_window = window_menu(screen_width, screen_height);

    while (running) {
        // Gestion des événements
        if (nb_window == 0) {    // Bouton Quitter cliqué --> stopper le programme
            running = 0;
        }
        else if (nb_window == 1) {    // La fenetre menu est ouverte
            nb_window = window_play(screen_width, screen_height);
            running = 0;
        }
        else if (nb_window == 2) {
            running = 0;
        }
    }


    TTF_Quit();
    SDL_Quit();

    return 0;
}
