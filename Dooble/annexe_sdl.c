#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>


- Chaque fenetre doit avoir son propre renderer pour pouvoir etre modifier (nb : on peut malgré tout afficher une fenetre sans déclarer de renderer, mais elle sera sans rien)
- Un renderer est ce qui va permettre d'afficher des trucs sur la fenêtres

SDL_CreateWindow( nom fenetre, 
                  position fenetre en x,
                  position fenetre en y, 
                  width fenetre, height fenetre, 
                  type d'affichage  ->SHOWN pour afficher direct, HIDDEN pour cacher direct
                                                                                                              

SDL_CreateRenderer(  nom fenetre sur laquelle il va travailler,
                     index pilote rendu            -> -1 pour laisser l'OS choisir le meilleur
                     les flags/options de rendu    -> SDL_RENDERER_ACCELERATED pour utliser le GPU (+ rapide)
