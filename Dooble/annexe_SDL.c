#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <SDl_ttf.h>

// Chaque fenetre doit avoir son propre renderer (= moteur de rendu) pour pouvoir y afficher des choses
// Nb : une fenetre peut malgré tout être afficher sans renderer, mais alors il n'y aura rien

// Toujours faire un SDL_RenderPresent(renderer1); dans la boucle principale pour actualiser en permanence la fenetre et conserver les données associés.

SDL_Init(SDL_INIT_EVERYTHING);    // Permet d'initialiser tout le SDL

SDL_DisplayMode screen;          // Structure permettant de stocker des données sur un écran, notamment height (.h) et width (.w)
SDL_GetCurrentDisplayMode(0, &screen);  // Permet de récuperer les infos de l'écran (indice 0 -> l'écran princiale) et les stocker dans la structure screen

SDL_Window* window1      // Strucuture permmettant de stocker des infos pour une fenetre 
SDL_CreateWindow(   "Dooble",                  // Titre
                    SDL_WINDOWPOS_UNDEFINED,   // Position fenetre en x (ici pas défini -> le OS va choisir la meilleur postion
                    SDL_WINDOWPOS_UNDEFINED,   // Idem pour y
                    400, 400,                  // width et height de la fenetre
                    SDL_WINDOW_SHOWN);         // Les flags (option de la fenetre) : FULLSCRENN, HIDDEN, SHOWN, RESIZABLE...

SDL_Renderer* renderer1    //Structure permettant de stocker des infos pour un renderer (= moteur de rendu), qui va permettre de faire un affichage sur une fenetre
SDL_CreateRenderer(window1,                   // Nom de la fenetre sur laquelle il va travailler
                  -1,                         // Index du pilote de rendu à utiliser, mettre -1 pour laisser le OS choisir le meilleur
                  SDL_RENDERER_ACCELERATED);  // Les flags (options), ici permet d'utiliser le GPU afin d'avoir un rendu plus rapide 

SDL_SetRenderDrawColor(renderer1, 255, 0, 255, 255); // Définit couleur que va utiliser le renderer, avec en parametre le renderer, couleurs RGD + a (=transparence ?)
SDL_RenderClear(renderer1);       // Colorie tout les pixels de la fenetre dont s'occupe le renderer en parametre MAIS n'actualise pas la fenetre
SDL_RenderPresent(renderer1);     // Mets a jour la fenetre du renderer afin de pouvoir afficher les modifications faites sur cette dernieree

SDL_Event event;      // Structure permettant de stocker des informations sur les événements dans SDL (entrées clavier, mouvements de souris,événements de fenêtre, ...)
SDL_PollEvent(&event) // Vérifie si evenement en attente. Si oui, il est retiré de file d'attente et stocké dans event. Retourne 1 si un événement a été récupéré, ou 0 sinon
event.key.keysym.sym  // permet d'identifier la touche spécifique qui a été pressée ou relâchée lors d'un événement clavier

SDL_DestroyRenderer(renderer1);  // Permet de détruire proprement un renderer (faire de préférence avant de détruire la fenetre associé)
SDL_DestroyWindow(window1);     // Permet de détruire proprement une fenetre (faire de préférence apres avoir détruit le renderer associé)
SDL_Quit();                      // Permet de quitter proprement SDL