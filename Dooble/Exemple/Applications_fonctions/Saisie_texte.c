#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);
    SDL_Window* window = SDL_CreateWindow("Test",
                                                SDL_WINDOWPOS_CENTERED,
                                                SDL_WINDOWPOS_CENTERED,
                                                screen.w, screen.h,
                                                SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 60);

    SDL_Rect text_box = {400,400,600,100};

    SDL_Event event;
    int running = 1;
    int is_active = 0;
    char* input_text = malloc(sizeof(char)*50);
    input_text[0] = '\0';
    char* saved_text = malloc(sizeof(char)*50);
    saved_text[0] = '\0';

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
            activate_input_box(event, &is_active, input_text, saved_text, text_box);
        }
        SDL_SetRenderDrawColor(renderer, 150,100,200,255);
        SDL_RenderClear(renderer);

        SDL_Color box_color = {255,255,255,255};
        SDL_Color font_color = {0,0,0,255};
        maj_input_box(renderer, text_box, box_color, input_text,font, font_color);

        SDL_RenderPresent(renderer);
    }
    printf("%s %s",saved_text, input_text);
    free(input_text);
    free(saved_text);
}
