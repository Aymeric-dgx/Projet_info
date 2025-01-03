#ifndef OPTION_C
#define OPTION_C


void window_menu(SDL_DisplayMode screen, int* nb_window);
void options_window(SDL_DisplayMode screen, int* nb_window, int* nb_joueurs_at_end, int* time_at_end, int* nb_words_at_end, int* gamemode_at_end, char** names_player_save_at_end);
void window_play_solo(SDL_DisplayMode screen, int* nb_window, int nb_words, int play_time, int* score);
void window_play_multi(SDL_DisplayMode screen, int nb_player, int nb_words, int play_time, int* scores, char** players_name, int* nb_window);

// Fonctions pour la fenetre play
void generate_list_solo(int nb_words, char** player_list, char** common_list, char* common_word, int init_list);
void generate_list_multi(int nb_words, int nb_player, char*** all_player_list, char** common_list, int init, int winning_player);





// Petite fonctions utilitaires
int click_in_rect(SDL_Rect rect);

void edit_progress_bar_with_click(SDL_Rect bar, SDL_Rect* sub_bar, float* ratio_bar);
void edit_progress_bar_with_ratio(SDL_Rect bar, SDL_Rect* sub_bar, float ratio);
void maj_progress_bar(SDL_Renderer* renderer, SDL_Rect bar, SDL_Rect sub_bar, SDL_Color bar_color, SDL_Color sub_bar_color);

void activate_input_box(SDL_Event event, int* is_active, char* input_text, char* saved_text, SDL_Rect input_box);
void maj_input_box(SDL_Renderer* renderer, SDL_Rect box, SDL_Color box_color, char* input_text, TTF_Font* font, SDL_Color font_color);

void create_button(SDL_Renderer* renderer, SDL_Rect button_rect, char text[100], TTF_Font* font, SDL_Color font_color, SDL_Color button_rect_color) ;

#endif();
