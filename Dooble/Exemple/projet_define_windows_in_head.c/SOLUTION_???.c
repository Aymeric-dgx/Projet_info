//Plutot que de s emmerder a retourenr un pointeur de pointeur de strucuture et tout le bordel, il y aurait beacoup plus simple, on ferait plutot :

#include <SDL.h>
#include <SDL_ttf.h>

int verif_showed_window(SDL_Window* window_showed, SDL_Window* windows[], int tab_size) {
    int comp_hidden_window = 0;
    if (SDL_GetWindowFlags(window_showed) & SDL_WINDOW_SHOWN) {
        for (int i=0; i < tab_size; i++) {
            if (SDL_GetWindowFlags(windows[i]) & SDL_WINDOW_HIDDEN) {
                comp_hidden_window+=1;
            }
        }
    }
    if (comp_hidden_window==tab_size-1) {
        return 1;
    }
    return 0;
}


// On doit créer les fenetre et renderer ici car on devra verifier si elle sont ouverte ou non ici  -->  on doit donc les avoir ici
// Ou sinon
// On créer des variables globales pour les fenetres et les renderer
// Ou encore
// On créer ici des variable qui valent 1 ou 0 et les fonctions qui créer/modifient les fenetres et les renderer renvoient 1 ou 0



int main() {

    // Initialisation de SDL et de SDL_ttf
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    int begining = 1;   // Pour "enclencher" les boucles, on dit artificellement que le menu est ouvert afin de lancer la focntion create_menu()
    int running = 1;    // Mais du coup (pour begining) p
    SDL_Event event;

    while (running) {
        while(SDL_PollEvent(&event)) {
            if (begining == 1) {
                create_menu();  // Créer la fonction menu qui contient tout le code du menu
                begining = 0;
            }
            if (verif_showed_window())
        }
    }
    return 0;
}
