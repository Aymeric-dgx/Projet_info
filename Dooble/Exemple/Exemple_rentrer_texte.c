#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define MAX_TEXT 256

int main(int argc, char* argv[]) {
    // Initialisation de SDL et SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        printf("Erreur d'initialisation de SDL ou SDL_ttf : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Zone de Texte",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          800, 600,
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        printf("Erreur de création de la fenêtre : %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        printf("Erreur de création du renderer : %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police : %s\n", TTF_GetError());
        return 1;
    }

    char inputText[MAX_TEXT] = "";
    char savedText[MAX_TEXT] = ""; // Variable pour stocker le texte après appui sur Enter
    SDL_StartTextInput();

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = NULL;
    SDL_Texture* textTexture = NULL;

    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_TEXTINPUT) {
                strcat(inputText, event.text.text);
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0) {
                    inputText[strlen(inputText) - 1] = '\0';
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    // Copier le texte saisi dans savedText
                    strncpy(savedText, inputText, MAX_TEXT);
                    printf("Texte sauvegardé : %s\n", savedText);
                    inputText[0] = '\0'; // Réinitialiser inputText pour permettre une nouvelle saisie
                }
            }
        }

        // Rendu du texte seulement si inputText n'est pas vide
        if (strlen(inputText) > 0) {
            textSurface = TTF_RenderText_Solid(font, inputText, textColor);
            if (textSurface == NULL) {
                printf("Erreur de création de la surface de texte : %s\n", TTF_GetError());
                break;
            }

            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            if (textTexture == NULL) {
                printf("Erreur de création de la texture de texte : %s\n", SDL_GetError());
                break;
            }

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

    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
