// Espace pour y deposer des petites fonctions qu on a fait (ne pas oublier de les déclarer dans option.h)
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"
#include <string.h>
#include <stdio.h>
#include "option.h"

/* Fonction actuellement stockées : 
    - click_in_rect : vérifie si le clic est dans rectangle ou non
    
    - edit_progress_bar_with_click : modifie une barre de progression en fonction d'un clic
    - edit_progress_bar_with_ratio : modifie une barre de progression en fonction d'un pourcentage demandé
    - maj_progress_bar : mets à jour visuellement une barre de progression

    - activate_input_box : active/désactive une box de saisie (si clic dans rect ou non) et modifie simplement la chaine de caractère prévu pour sauvegardé la saisie
    - maj_input_box : mets à jour visuellement une box de saisie

    - create_button : créer un bouton avec le texte centré
*/

// Fonction qui verifie si un click est dans un rectangle, et renvoie 1 ou 0
// Parametre : posistions x et y de la souris, le rectangle dans lequelle on veut verifier le click
int click_in_rect(int mouse_x, int mouse_y, SDL_Rect rect) {
    if (mouse_x >= rect.x && mouse_x <= rect.x+rect.w &&
        mouse_y >= rect.y && mouse_y <= rect.y+rect.h) {
        return 1;
    } else {
        return 0;
    }
}


/*
Modifie la barre de progression en fonction d'un click (ne mets pas à jour, juste modifie les dimension du sub_rect) :
    - Le rect pour la barre (créé et placé en amont)
    - Un pointeur vers le rect pour la progression (créé en amont)
    - Le x et y de la souris, récupéré en ammont
    - Un pointeur pour y enregistré le % de la barre remplie en fontion du clic. Si pas néscessaire de récupérer le % dans le main, mettre NULL
 */
void edit_progress_bar_with_click(SDL_Rect bar, SDL_Rect* sub_bar, int mouse_x, int mouse_y, int* ratio_bar) {
    if (click_in_rect(mouse_x, mouse_y, bar)) {
        sub_bar->x = bar.x;
        sub_bar->y = bar.y;
        sub_bar->w = mouse_x - bar.x;
        sub_bar->h = bar.h;
        
        if (ratio_bar != NULL) {
            float f_mouse_x = mouse_x, f_barre_w = bar.w, f_barre_x = bar.x;
            float tmp_ratio= ( (f_mouse_x - f_barre_x) / f_barre_w ) *100;
            *ratio_bar = tmp_ratio;
        }
    }
}

/*
Modifie la barre de progression en fonction d'un % souhaité (ne mets pas à jour, juste modifie les dimension du sub_rect) :
    - Le rect pour la barre (créé et placé en amont)
    - Un pointeur vers le rect pour la progression (créé en amont)
    - Le % de la barre qu'on veut remplir
*/
void edit_progress_bar_with_ratio(SDL_Rect bar, SDL_Rect* sub_bar, int ratio) {
    if(ratio>=0 && ratio<=100) {
        sub_bar->x = bar.x;
        sub_bar->y = bar.y;
        sub_bar->w = (ratio*bar.w) / 100;
        sub_bar->h = bar.h;
    }
}

/*
Mets à jour la barre de progression :
    - Renderer
    - Rect pour la barre (créé et placer en ammont)
    - Rect pour la progression (créé et modifié en ammont)
    - Couleur du fond de la barre principale
    - Couleur de la barre montrant la progression
 */
void maj_progress_bar(SDL_Renderer* renderer, SDL_Rect bar, SDL_Rect sub_bar, SDL_Color bar_color, SDL_Color sub_bar_color) {
    SDL_SetRenderDrawColor (renderer, bar_color.r, bar_color.g, bar_color.b, bar_color.a);
    SDL_RenderFillRect(renderer, &bar);
    SDL_SetRenderDrawColor (renderer, sub_bar_color.r, sub_bar_color.g, sub_bar_color.b, sub_bar_color.a);
    SDL_RenderFillRect (renderer, &sub_bar);
}



/*Fonction à mettre dans la gestion des evenements "nu", sans condition dans la boucle de gestions des evenemts (verifie lui meme la condtion des evenement)
Va verifier si le clic est dans la box de saisie, et va modifier un pointeur de str avec les entrées du clavier
Mise en place néscessaire :
    - Donner le POINTEUR de l'event qui a été créé dans le main
    - Donner le POINTEUR de is_active pour "sauvegarder" dans le main si la box de saisie est cliqué ou pas
    - Donner le POINTEUR (définit avec malloc) de la chaine de caractère à modifier (et non pas un tableau de char car ne peut pas etre return)µ
            ATTENTION -> initialiser le premier caractere (p_texte[0] = '\0')
    - Le rect dans lequelle doit etre saise le texte (pour détecter si le clic active ou non la box de saisie)
 */


void activate_input_box(SDL_Event* event, int* is_active, char* input_text, SDL_Rect input_box) {
    if(event->type == SDL_TEXTINPUT && *is_active ==1) {
        strcat(input_text, event->text.text);
    }
    else if(event->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(click_in_rect(x, y, input_box)) *is_active = 1;
        else *is_active = 0;
    }
    else if(event->type == SDL_KEYDOWN && *is_active == 1) {
        if (event->key.keysym.sym == SDLK_BACKSPACE && strlen(input_text)>0) {
            input_text[strlen(input_text) - 1] = '\0';
        }
        else if(event->key.keysym.sym == SDLK_RETURN) {
            input_text[0] = '\0';
        }
    }
}


/*
Fonction qui met à jour la zone de saisie de texte :
    - Renderer
    - Rect pour la box de saisie (créé et placer en amont)
    - Rect pour la zone de texte (créé avant)
    - Couleur du fond de la box de saisie
    - Police
    - Couleur de police
    - Pointeur pour communiquer le texte saisie (et qui a été saisie) entre la fonction et le main
 */
void maj_input_box(SDL_Renderer* renderer, SDL_Rect box, SDL_Rect* text_box, SDL_Color box_color, TTF_Font* font, SDL_Color font_color, char* input_text) {

    SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
    SDL_RenderFillRect(renderer, &box);

    if(strlen(input_text)>0) {
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, input_text, font_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        text_box->x = box.x+box.w/10;
        text_box->y = box.y + (box.h - text_surface->h) / 2;
        text_box->w = text_surface->w;
        text_box->h = text_surface->h;

        SDL_RenderCopy(renderer, text_texture, NULL, text_box);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text_surface);
    }
}



/*
Fonction qui va permettre de créer et gérer + facilement des boutons, en affichant le texte au centre :
    - Renderer
    - Rect pour placer le bouton (créé et placé en ammont)
    - Rect pour placer le texte (créé en ammont)
    - Le texte à écrire dedans
    - La police
    - Couleur de la police
    ' Couleur du fond
 */

void create_button(SDL_Renderer* renderer, SDL_Rect button_rect, SDL_Rect text_rect, char text[100], TTF_Font* font, SDL_Color font_color, SDL_Color button_rect_color) {

    SDL_SetRenderDrawColor(renderer, button_rect_color.r, button_rect_color.g, button_rect_color.b, button_rect_color.a);
    SDL_RenderFillRect(renderer, &button_rect);
    SDL_Surface* tmp_surface = TTF_RenderText_Solid(font, text, font_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    text_rect.x = button_rect.x + button_rect.w/2 - tmp_surface->w/2;
    text_rect.y = button_rect.y + button_rect.h/2 - tmp_surface->h/2;
    text_rect.w = tmp_surface->w;
    text_rect.h = tmp_surface->h;

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(tmp_surface);
    SDL_DestroyTexture(text_texture);
}
