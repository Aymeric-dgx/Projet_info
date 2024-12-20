#ifndef OPTION_C
#define OPTION_C


void window_menu(SDL_DisplayMode screen, int* nb_window);
void options_window(SDL_DisplayMode screen, int* nb_window, int* nb_joueurs_at_end, int* time_at_end, int* nb_words_at_end, int* gamemode_at_end, char** names_player_save_at_end);


// Petite fonctions utilitaires
int click_in_rect(SDL_Rect rect);

void edit_progress_bar_with_click(SDL_Rect bar, SDL_Rect* sub_bar, float* ratio_bar);
void edit_progress_bar_with_ratio(SDL_Rect bar, SDL_Rect* sub_bar, int ratio);
void maj_progress_bar(SDL_Renderer* renderer, SDL_Rect bar, SDL_Rect sub_bar, SDL_Color bar_color, SDL_Color sub_bar_color);

void activate_input_box(SDL_Event event, int* is_active, char* input_text, char* saved_text, SDL_Rect input_box);
void maj_input_box(SDL_Renderer* renderer, SDL_Rect box, SDL_Color box_color, char* input_text, TTF_Font* font, SDL_Color font_color);

void create_button(SDL_Renderer* renderer, SDL_Rect button_rect, char text[100], TTF_Font* font, SDL_Color font_color, SDL_Color button_rect_color) ;

#endif();
