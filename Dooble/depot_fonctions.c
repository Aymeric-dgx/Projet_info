// Espace pour y deposer des petites fonctions qu on a fait (ne pas oublier de les déclarer dans option.h)
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"
#include <string.h>
#include <stdio.h>
#include "option.h"

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
    - Le rect pour la barre (qu'on aura créé et placé en amont)
    - Un pointeur vers le rect pour la progression (qu'on aura créé en amont)
    - Le x et y de la souris, récupéré en ammont
    - Un pointeur pour y enregistré le % de la barre remplie en fontion du clic. Si pas néscessaire, mettre NULL
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
    - Le rect pour la barre (qu'on aura créé et placé en amont)
    - Un pointeur vers le rect pour la progression (qu'on aura créé en amont)
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
Mets à jour la barre de progression (pensez à faire le renderClear dans le main) :
    - Un pointeur du renderer (mettre simplement le renderer, car déja défini comme pointeur dans le main)
    - Le rect pour la barre (créé et placer en ammont)
    - Le rect pour la progression (créé puis modifié à l'aide des fonctions précédentes en ammont)
    - La couleur (SDL_Color) définit en ammont pour les 2 rect
 */
void maj_progress_bar(SDL_Renderer* renderer, SDL_Rect bar, SDL_Rect sub_bar, SDL_Color bar_color, SDL_Color sub_bar_color) {
    SDL_SetRenderDrawColor (renderer, bar_color.r, bar_color.g, bar_color.b, bar_color.a);
    SDL_RenderFillRect(renderer, &bar);
    SDL_SetRenderDrawColor (renderer, sub_bar_color.r, sub_bar_color.g, sub_bar_color.b, sub_bar_color.a);
    SDL_RenderFillRect (renderer, &sub_bar);
}
