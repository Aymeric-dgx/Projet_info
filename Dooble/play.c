//Le code ne fonctionne pas mais c'est a cause de mon essaie pour rentrer du texte, aller voir exemple rentrer de texte dans le fichier Exemple

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <stdio.h>

#define MAX_TEXT 256

int window_play(screen_width, screen_height) {
    // Créer la fenêtre play et le renderer
    SDL_Window* window_play = SDL_CreateWindow("Dooble",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width, screen_height,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_play = SDL_CreateRenderer(window_play , -1, SDL_RENDERER_ACCELERATED);

    char inputText[MAX_TEXT] = "";
    char savedText[MAX_TEXT] = ""; // Variable pour stocker le texte après appui sur Enter
    SDL_StartTextInput();

    int returning = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running =0;
            }
            else if (event.type == SDL_KEYDOWN) {
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
        SDL_SetRenderDrawColor(renderer_play, 0, 0, 50,255);
        SDL_RenderClear(renderer_play);

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
        SDL_RenderPresent(renderer_play);
    }
    SDL_DestroyWindow(window_play);
    SDL_DestroyRenderer(renderer_play);
    return 0;
}
