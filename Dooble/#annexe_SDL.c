#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <SDl_ttf.h>

// Au moment de la déclaration de variable, on peut mettre const avant pour préciser que ces variables ne peuvent pas etre modifié

// Chaque fenetre doit avoir son propre renderer (= moteur de rendu) pour pouvoir y afficher des choses
// Nb : une fenetre peut malgré tout être afficher sans renderer, mais alors il n'y aura rien

// Toujours faire un SDL_RenderPresent(renderer1); dans la boucle principale pour actualiser en permanence la fenetre et re-actualisé les données associés.

// Nb pour strucutre :  Utiliser ->w avec un pointeur vers une structure est équivalent à utiliser .w avec une structure directe. La différence principale est 
// que -> est utilisé pour accéder aux membres d'une structure via un pointeur, tandis que . est utilisé pour accéder aux membres d'une structure directement.
// Exemple : 
/*Avec un pointeur :
SDL_Surface* titleSurface = TTF_RenderText_Solid(bigFont, "Bienvenue", textColor);
int largeur = titleSurface->w; // Utilise -> pour accéder à la largeur via un pointeur

Sans pointeur :
SDL_Surface titleSurface;
int largeur = titleSurface.w; // Utilise . pour accéder à la largeur directement 
*/

// On peut écrire un if sans les { } si il ne contient qu'une instruction, par exemple : if (event.type == SDL_QUIT) running = 0;

SDL_Init(SDL_INIT_EVERYTHING);    // Permet d'initialiser tout le SDL

SDL_DisplayMode screen;          // Structure permettant de stocker les données d'un écran, notamment height (.h) et width (.w)
SDL_GetCurrentDisplayMode(0, &screen);  // Permet de récuperer les infos de l'écran (indice 0 -> l'écran princiale) et les stocker dans la structure screen

SDL_Window* window1      // Strucuture permmettant de stocker des infos pour une fenetre 
SDL_CreateWindow(   "Dooble",                  // Titre
                    SDL_WINDOWPOS_UNDEFINED,   // Position fenetre en x (ici pas défini -> le OS va choisir la meilleur postion)
                    SDL_WINDOWPOS_UNDEFINED,   // Idem pour y
                    400, 400,                  // width et height de la fenetre
                    SDL_WINDOW_SHOWN);         // Les flags (option de la fenetre) : FULLSCRENN, HIDDEN, SHOWN, RESIZABLE...

SDL_GetWindowFlags(window1);     // Récupere le flag de la fenetre -> peut etre utile pour verifier si une fenetre est montrée (SDL_WINDOW_SHOWN) ou caché (SDL_WINDOW_HIDDEN)
// Atention c'est un peu particulier : GetWindowFlags renvoie un masque de bits (= une "liste" de bits, par exemple 0001011) et il faut le comparerer avec les differents
// flags (SHOWN, HIDDEN, ...) qui ont eux aussi un masque de bits   -> on fait un ET logique (&) entre ces deux masques la 
// Forme  ->  if (SDL_GetWindowFlags(window1) & SDL_WINDOW_SHOWN) {    
//                  bla bla};

SDL_Renderer* renderer1    //Structure permettant de stocker des infos pour un renderer (= moteur de rendu), qui va permettre de faire un affichage sur une fenetre
SDL_CreateRenderer(window1,                   // Nom de la fenetre sur laquelle il va travailler
                  -1,                         // Index du pilote de rendu à utiliser, mettre -1 pour laisser le OS choisir le meilleur
                  SDL_RENDERER_ACCELERATED);  // Les flags (options), ici permet d'utiliser le GPU afin d'avoir un rendu plus rapide 

SDL_SetRenderDrawColor(renderer1, 255, 0, 255, 255); // Définit couleur que va utiliser le renderer, avec en parametre le renderer, couleurs RGD + a (=transparence ?)
SDL_RenderClear(renderer1);       // Colorie tout les pixels de la fenetre dont s'occupe le renderer en parametre MAIS n'actualise pas "visuellelement" la fenetre
SDL_RenderPresent(renderer1);     // Mets a jour la fenetre du renderer afin de pouvoir afficher les modifications faites sur cette derniere

SDL_Rect* rect;     // Strucure permettant de stocker des infos pour un rectangle, .x .y .w et .h
SDL_RenderDrawRect(renderer, &rect);  // Déssine coutour du rectangle (largeur 1 pixel) avec la couleur du renderer (useless sauf si rectangle collés pour les différencier)
SDL_RenderFillRect(renderer, &rect);   // Rempli le rectangle avec la couleur du renderer
// Pour faire des contours plus épais, écrire une foncton qui va dessiner plusieurs petits rectangele sur les contours d'un plus grand
SDL_Rect buttons[3];  // Créer un tableau buttons de 3 structure SDL_Rect (= créer un tableau contenant 3 rectangles)


TTF_Font* bigFont  // Structure permettant de stocker des infos pour des polices TTF 
TTF_OpenFont("../police/arial.ttf", 64)  // Utilisé pour charger une police TTF à partir d'un fichier : chemin d'accès a la police, taille de la police
SDL_Color textColor    // Strucutre permettant de stocker des infos pour une couleur : .r (red), .g (green), .b (blue), .a (alpha = oppacité)
SDL_Surface* titleSurface    // Représente une image en mémoire, par exemmple : chargement d'image depuis un path, affichage d'un texte, manipulation de pixels, (blitting ?)
TTF_RenderText_Solid(bigFont, "Bienvenue", textColor);    // Return du texte en utilisant : police, texte, couleur du texte
SDL_Texture* titleTexture  // Used to represent image, but + fast que SDL_Surface (directement utilisé par GPU), utilisé pour : rendu sur écran, performances, manip d'images
SDL_CreateTextureFromSurface(renderer, titleSurface);  // Convertit surface en texture pour rendu + rapide (GPU) : renderer avec qui travailler, surface qu'on veut convertir
SDL_FreeSurface(titleSurface);  // Free la mémoire alloué à une surface (à libérer par exemple après avoir créer une texture via cette surface, et dont on a donc + besoin) 
SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect); // Copie une texture dans le rendu actuel (a peu près équivalent à SDL_RenderPresent mais pour des texture)
// parametres : renderer avec qui travailler, texture qu'on veut copier, si on veut récup. qu'une partie de la texture -> donner pointeur vers un rect de la taiile 
// qu'on souhaite, pointeur vers un rect où l'on va copier la texture
SDL_DestroyTexture(titleTexture);  // Detruire proprement la texture
TTF_CloseFont(bigFont);   // Feremr et libérer la mémoire associée à une police (TTF_Font) précédemment ouverte avec TTF_OpenFont
TTF_Quit();    // Fermer et nettoyer la lib SDL_ttf

SDL_Event event;      // Structure permettant de stocker des informations sur les événements dans SDL (entrées clavier, mouvements de souris,événements de fenêtre, ...)
SDL_PollEvent(&event) // Vérifie si evenement en attente. Si oui, il est retiré de file d'attente et stocké dans event. Retourne 1 si un événement a été récupéré, ou 0 sinon
event.key.keysym.sym  // permet d'identifier la touche spécifique qui a été pressée ou relâchée lors d'un événement clavier
int mouseX = event.button.x, mouseY = event.button.y; // (à verifier), si event == MOUSEBUTTONDOWN (ou UP), permet de récupérer les coordonnées de la souris

SDL_Log("Bouton 'Jouer' cliqué !") // Ecrit un message de journalisation dans la console

SDL_DestroyRenderer(renderer1);  // Permet de détruire proprement un renderer (faire de préférence avant de détruire la fenetre associé)
SDL_DestroyWindow(window1);     // Permet de détruire proprement une fenetre (faire de préférence apres avoir détruit le renderer associé)
SDL_Quit();                      // Permet de quitter proprement SDL