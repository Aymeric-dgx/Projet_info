#ifndef OPTION_C
#define OPTION_C

int window_menu();
int window_play();

// Petite fonctions utilitaires
int click_in_rect(int mouse_x, int mouse_y, SDL_Rect rect)

void edit_progress_bar_with_click(SDL_Rect bar, SDL_Rect* sub_bar, int mouse_x, int mouse_y, int* ratio_bar)
void edit_progress_bar_with_ratio(SDL_Rect bar, SDL_Rect* sub_bar, int ratio)
void maj_progress_bar(SDL_Renderer* renderer, SDL_Rect bar, SDL_Rect sub_bar, SDL_Color bar_color, SDL_Color sub_bar_color)77

void activate_input_box(SDL_Event* event, int* is_active, char* input_text, SDL_Rect input_box)

#endif();
