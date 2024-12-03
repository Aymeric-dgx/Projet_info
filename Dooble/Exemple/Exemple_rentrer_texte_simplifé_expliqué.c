#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define MAX_TEXT 256

int main(int argc, char* argv[]) {

    // Initialisation de SDL et SDL_ttf
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Création fenetre + renderer + police
    SDL_Window* window = SDL_CreateWindow("Zone de Texte",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 24);

    // Création des chaines de caratere + SDL_StartTextInput()
    char inputText[MAX_TEXT] = "";
    char savedText[MAX_TEXT] = ""; // Variable pour stocker le texte après appui sur Enter
    SDL_StartTextInput();

    // Init couleur texte + création pointeur surface et texture
    SDL_Color textColor = {255, 255, 255, 255}; //blanc
    SDL_Surface* textSurface = NULL;
    SDL_Texture* textTexture = NULL;

    // Début boucle principale
    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            // Si clavier pour du texte appuyé
            else if (event.type == SDL_TEXTINPUT) {
                strcat(inputText, event.text.text);
            // Si touche autre que texte (ou pour du texte aussi mais pas l'objectif ici)
            } else if (event.type == SDL_KEYDOWN) {
                // Suppression du dernier caractere si backspace appuyé (on remplace la letre par le caractère nul --> fin de chaine juste avant ce caractère "supprimé")
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0';
                // Si Entrée appuyé...
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    // Copier le texte saisi dans savedText
                    strncpy(savedText, inputText, MAX_TEXT);
                    printf("Texte sauvegardé : %s\n", savedText);
                    inputText[0] = '\0'; // Réinitialiser inputText pour permettre une nouvelle saisie
                    // Puisque \0 définit la fin de la chaine --> on le met donc au tout début
                }
            }
        }

        // Rendu du texte seulement si inputText n'est pas vide
        if (strlen(inputText) > 0) {
            // Création surface puis texture avec le texte pour l'afficher sur la fenetre
            textSurface = TTF_RenderText_Solid(font, inputText, textColor);
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            // Création du rectangle pour placer le texte
            SDL_Rect textRect = {50, 50, textSurface->w, textSurface->h};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Effacer l'écran en noir
            SDL_RenderClear(renderer);

            // Dessiner la zone de texte
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_DestroyTexture(textTexture);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Effacer l'écran en noir
            SDL_RenderClear(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    // Destruction
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
