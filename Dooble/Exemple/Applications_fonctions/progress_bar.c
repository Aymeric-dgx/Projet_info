#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "option.h"

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    SDL_Window* window = SDL_CreateWindow("Dooble",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        screen.w, screen.h,
                                        SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window , -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect bar = {800,100,200,50};
    SDL_Rect sub_bar;
    SDL_Rect* p_sub_bar = &sub_bar;
    int ratio;
    int*p_ratio = &ratio;

    int running = 1;
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                edit_progress_bar_with_click(bar, p_sub_bar, x, y, p_ratio);
            }
            if(event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Color bar_color = {255,255,0,255};
        SDL_Color sub_bar_color = {255,0,0,255};
        // Si on veut remplir la barre de progression à 82% :
        //edit_progress_bar_with_ratio(bar, p_sub_bar, 82);
        maj_progress_bar(renderer, bar, sub_bar, bar_color, sub_bar_color);

        SDL_RenderPresent(renderer);
    }
    printf("%d", ratio);
}
