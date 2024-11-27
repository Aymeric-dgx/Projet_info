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
