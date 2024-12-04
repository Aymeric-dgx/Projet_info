#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 5 // Longueur maximale de l'entrée utilisateur

int window_menu(int screen_width, int screen_height) {
    // Initialisation de la fenêtre et du renderer
    SDL_Window* window_menu = SDL_CreateWindow("Option",
                                               SDL_WINDOWPOS_CENTERED,
                                               SDL_WINDOWPOS_CENTERED,
                                               screen_width, screen_height,
                                               SDL_WINDOW_SHOWN);
    if (!window_menu) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer_menu = SDL_CreateRenderer(window_menu, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_menu) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window_menu);
        return -1;
    }

    // Chargement des polices
    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 24); // Police pour texte
    if (!font) {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer_menu);
        SDL_DestroyWindow(window_menu);
        return -1;
    }

    // Définir les couleurs
    SDL_Color textColor = {255, 255, 255, 255}; // Couleur du texte
    SDL_Color boxColor = {255, 255, 255, 255};  // Couleur de la barre de saisie

    // Texte "Option de jeux"
    const char* title = "Option de jeux";
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, title, textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer_menu, titleSurface);
    SDL_Rect titleRect = {
        screen_width / 2 - titleSurface->w / 2,
        screen_height / 4 - titleSurface->h / 2,
        titleSurface->w,
        titleSurface->h
    };
    SDL_FreeSurface(titleSurface);

    // Définir la position et la taille de la barre de saisie
    SDL_Rect inputBox = {
        screen_width / 2 - 150,  // Centrer horizontalement
        screen_height / 2 - 25, // Position verticale
        300,                    // Largeur de la barre
        50                      // Hauteur de la barre
    };

    // Variables de saisie
    char inputText[MAX_INPUT_LENGTH] = ""; // Stocke le texte saisi
    int inputComplete = 0;
    int enteredValue = 0;
    int running = 1;
    int isActive = 0;  // Détermine si la barre de saisie est activée (cliquée)

    SDL_StartTextInput(); // Activer la saisie texte
    SDL_Event event;

    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_TEXTINPUT && isActive) {
                if (strlen(inputText) < MAX_INPUT_LENGTH - 1) {
                    strcat(inputText, event.text.text); // Ajouter le caractère saisi
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Vérifier si le clic est dans la barre de saisie
                int mouseX = event.button.x, mouseY = event.button.y;
                if (mouseX >= inputBox.x && mouseX <= inputBox.x + inputBox.w &&
                    mouseY >= inputBox.y && mouseY <= inputBox.y + inputBox.h) {
                    isActive = 1; // Activer la saisie lorsque la barre est cliquée
                } else {
                    isActive = 0; // Désactiver la saisie si le clic n'est pas dans la barre
                }
            } else if (event.type == SDL_KEYDOWN && isActive) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0'; // Supprimer le dernier caractère
                } else if (event.key.keysym.sym == SDLK_RETURN && strlen(inputText) > 0) {
                    inputComplete = 1; // Valider la saisie
                    enteredValue = atoi(inputText); // Convertir le texte en entier
                    printf("Valeur saisie : %d\n", enteredValue);
                    running = 0; // Quitter la boucle après validation
                }
            }
        }

        // Dessiner l'arrière-plan
        SDL_SetRenderDrawColor(renderer_menu, 0, 0, 50, 255); // Bleu foncé
        SDL_RenderClear(renderer_menu);

        // Dessiner le titre "Option de jeux"
        SDL_RenderCopy(renderer_menu, titleTexture, NULL, &titleRect);

        // Dessiner la barre de saisie
        SDL_SetRenderDrawColor(renderer_menu, boxColor.r, boxColor.g, boxColor.b, boxColor.a);
        SDL_RenderDrawRect(renderer_menu, &inputBox);

        // Dessiner le texte saisi
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, inputText, textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer_menu, textSurface);
            SDL_Rect textRect = {
                inputBox.x + 10,                  // Décalage à gauche
                inputBox.y + (inputBox.h - textSurface->h) / 2, // Centrer verticalement
                textSurface->w,
                textSurface->h
            };
            SDL_RenderCopy(renderer_menu, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }

        // Mise à jour de l'écran
        SDL_RenderPresent(renderer_menu);
    }

    // Libérer les ressources
    SDL_StopTextInput(); // Désactiver la saisie texte
    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer_menu);
    SDL_DestroyWindow(window_menu);

    return enteredValue; // Retourner la valeur saisie
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Obtenir les dimensions de l'écran
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    int screen_width = screen.w;
    int screen_height = screen.h;

    // Lancer le menu
    int userValue = window_menu(screen_width, screen_height);
    printf("Nombre final saisi : %d\n", userValue);

    TTF_Quit();
    SDL_Quit();
    return 0;
}
