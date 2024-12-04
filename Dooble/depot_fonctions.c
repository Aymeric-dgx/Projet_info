// Espace pour y deposer des petites fonctions qu on a fait (ne pas oublier de les déclarer dans option.h)
#include <SDL.h>
#include <SDL_ttf.h>
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
