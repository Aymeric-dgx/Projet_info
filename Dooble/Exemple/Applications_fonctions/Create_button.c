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

    SDL_Window* window = SDL_CreateWindow("Dooble",
                                             SDL_WINDOWPOS_CENTERED,
                                             SDL_WINDOWPOS_CENTERED,
                                             screen.w, screen.h,
                                             SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 64);


    int running = 1;
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Rect button_rect = {100,100,800,500};
        SDL_Rect text_rect = {200,200};
        SDL_Color font_color = {255,255,0,255};
        SDL_Color button_rect_color = {0,255,255,255};
        create_button(renderer, button_rect, text_rect, "Hello test", font, font_color, button_rect_color);
        SDL_RenderPresent(renderer);
    }
}
