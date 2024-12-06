
int main() {
    // Initialisation SDL + TTF (à retirer une fois "associé" avec le main, c est juste pour le test)
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    // Obtenir les dimensions de l'écran (idem, a retirer une fois "associé)
    SDL_DisplayMode screen;
    SDL_GetCurrentDisplayMode(0, &screen);

    SDL_Window* window_regles = SDL_CreateWindow("Dooble",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         screen.w, screen.h,
                                         SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer_regles = SDL_CreateRenderer(window_regles , -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("../police/arial.ttf", 60); // Police pour texte

    SDL_Rect box = {200,200,300,100};
    SDL_Rect rect_text;
    SDL_Rect* p_rect_text = &rect_text;

    SDL_Event event;
    SDL_Event* p_event = &event;
    int running = 1;
    char* input_text = malloc(sizeof(char)*50);
    input_text[0] = '\0';
    int is_active = 0;
    int* p_is_active = &is_active;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = 0;
            }
            activate_input_box(p_event, p_is_active, input_text, box);
        }

        SDL_Color box_color = {0,255,0,255};
        SDL_Color font_color = {255,0,255,255};

        maj_input_box(renderer_regles, box, p_rect_text, box_color, font, font_color, input_text);

        SDL_RenderPresent(renderer_regles);
    }
    free(input_text);
}
