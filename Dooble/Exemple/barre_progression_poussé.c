#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include "option.h"

#define MAX_INPUT_LENGTH 5

// Reste à faire :
// Pouvoir entrer manuellement (via une zone de texte) le temps désiré : actualisé temps sélectionné + barre de progression

int main() {

    // Initialisation SDL + TTF (à retirer une fois "associé" avec le main, c est juste pour le test)
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Obtenir les dimensions de l'écran (idem, a retirer une fois "associé)
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);


    // Création window + renderer + chargement police
    SDL_Window* window_regles = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_regles = SDL_CreateRenderer(window_regles , -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* police = TTF_OpenFont("../police/arial.ttf", (screen.w / 600) * 24); // Police grande taille (taille qui s'adapte à la taille de l'écan)


    // Création de la barre de progresssion
    SDL_Rect barre = {screen.w/5, screen.h/2, 1000, 200};


    // Création de la zone de saisie de texte
    SDL_Rect inputBox = {barre.x+barre.w/2, barre.y+1.5*barre.h, barre.w/3, barre.h/3};


    // Début boucle principale
    SDL_Event event;
    SDL_StartTextInput();
    int running = 1;
    int time = 60;
    int sub_barre_created = 0;  // Va permettre de vérifier si on a créé au moins une fois la barre de progression
    int changing_sub_barre = 0; // Va permettre de dire qu'on a modifié la position de la barre
    int input_box_active = 0;   // Dit si la box de saisie de texte est cliqué ou non
    char input_text[MAX_INPUT_LENGTH] = "";
    int wrong_nb = 0; // Va permettre de dire si le nombre saisie manuellement est valide ou pas

    while(running) {
        // Gestion des évenements
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);

                if (click_in_rect(mouse_x, mouse_y, barre)) {
                    // Caclul du temps en fonction du click
                    float f_mouse_x = mouse_x, f_barre_x = barre.x, f_barre_w = barre.w;    // On passe tout en float pour éviter les pb de division entière
                    float tmp = 60 + (f_mouse_x-f_barre_x) / f_barre_w * 120;   // // tps de base + %barre * temps max à ajouter
                    time = tmp ;   // On passe tmp (float) en int (temps)
                    printf("temps selectionne : %d\n",time);

                    sub_barre_created = 1;  // On dit qu'on a créer le sub_barre (utile uniquement la première fois)
                    changing_sub_barre = 1; // On dit qu'on a modifé la positon de la barre de progression
                }
                else if(click_in_rect(mouse_x, mouse_y, inputBox)) {
                    input_box_active = 1;
                }
                else {
                    input_box_active = 0;
                }
            }
            else if (event.type == SDL_TEXTINPUT && input_box_active) {
                if(strlen(input_text)>5) {
                    input_text[0] = '\0';
                }
                if(strlen(input_text)<MAX_INPUT_LENGTH-1) {
                    strcat(input_text, event.text.text);
                }
                wrong_nb = 0;
            }
            else if(event.type == SDL_KEYDOWN && input_box_active) {
                if(event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_text)<0) {
                    input_text[strlen(input_text)-1] = '\0';
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {                 // Peut etre ici, avec strlen(input_text)>0
                    if(atoi(input_text)<60 || atoi(input_text)>180) {
                        wrong_nb = 1;
                    } else {
                        time = atoi(input_text);    // Convertit le str en int
                        input_text[0] = '\0';
                    }
                }
            }
        }
        // Fin gestion des évenements

        // Remplisaage fond
        SDL_SetRenderDrawColor(renderer_regles, 0, 0, 0, 255);
        SDL_RenderClear(renderer_regles);


        // Remplissage de la barre en entier
        SDL_SetRenderDrawColor(renderer_regles, 150, 150, 150, 255);
        SDL_RenderFillRect(renderer_regles, &barre);


        // Remplissage de la barre pour afficher la progression (si elle existe)
        if (sub_barre_created) {
            int mouse_x, mouse_y;
            // Si on a changé la position de la barre, on modifie la position de la sourie (qui permet de définir les dimensions de la barre)
            // Sinon la barre reste la meme et est actualisé
            if (changing_sub_barre) {
                SDL_GetMouseState(&mouse_x, &mouse_y);
                changing_sub_barre = 0;
            }
            // On définit la barre (soit on la definit identique, ou soit on la definit avec les nouvelle dimensions), puis on la dessine
            SDL_Rect sub_barre = {barre.x, barre.y, mouse_x-barre.x, barre.h};
            SDL_SetRenderDrawColor(renderer_regles, 0, 255, 255, 255);
            SDL_RenderFillRect(renderer_regles, &sub_barre);
        }


        // Affichage du temps sélectionné
        SDL_Color show_time_color = {0,0,0,255};
        char number[10] = "";
        sprintf(number, "%d", time);
        strcat(number, " sec");
        SDL_Surface* tmp = TTF_RenderText_Solid(police, number, show_time_color);
        SDL_Texture* show_time = SDL_CreateTextureFromSurface(renderer_regles, tmp);
        SDL_Rect rect_show_time = {barre.x, 1.5*barre.y, tmp->w, tmp->h};
        SDL_FreeSurface(tmp);

        SDL_SetRenderDrawColor(renderer_regles,255,255,255,255);
        SDL_RenderFillRect(renderer_regles, &rect_show_time);
        SDL_RenderCopy(renderer_regles, show_time, NULL, &rect_show_time);
        SDL_DestroyTexture(show_time);


        // Affichage de la selection manuelle du temps
        SDL_SetRenderDrawColor(renderer_regles,255,255,255,255);
        SDL_RenderDrawRect(renderer_regles, &inputBox);
        SDL_Color color_text = {255,255,255,255};

        if (strlen(input_text)>0) {

            SDL_Surface* tmp;
            SDL_Texture* text_box;
            SDL_Rect rect_text_box;

            if (wrong_nb) {
                strcpy(input_text, "Entrer un nb entre 60 et 180");
                tmp = TTF_RenderText_Solid(police, input_text, color_text);
                text_box = SDL_CreateTextureFromSurface(renderer_regles, tmp);
                rect_text_box = (SDL_Rect){inputBox.x + inputBox.x / 50, inputBox.y + inputBox.h / 10, tmp->w, tmp->h};
            } else {
                tmp = TTF_RenderText_Solid(police, input_text, color_text);
                text_box = SDL_CreateTextureFromSurface(renderer_regles, tmp);
                rect_text_box = (SDL_Rect){inputBox.x + inputBox.x / 50, inputBox.y + inputBox.h / 10, tmp->w, tmp->h};
            }

            SDL_RenderCopy(renderer_regles, text_box, NULL, &rect_text_box);
            SDL_FreeSurface(tmp); // Libération de la surface
            SDL_DestroyTexture(text_box); // Libération de la texture après utilisation
        }


        // Màj
        SDL_RenderPresent(renderer_regles);
    }

    // Libération mémoire
    SDL_DestroyRenderer(renderer_regles);
    SDL_DestroyWindow(window_regles);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
}
