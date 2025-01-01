// Espace pour y deposer des fonctions qu on a fait (ne pas oublier de les déclarer dans option.h)
#include <SDL.h>
#include <SDL_ttf.h>
#include "option.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "option.h"




// Fonction qui verifie si un click est dans un rectangle, et renvoie 1 ou 0
// Parametre : le rectangle dans lequelle on veut verifier le click
int click_in_rect(SDL_Rect rect) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (x >= rect.x && x <= rect.x+rect.w &&
        y >= rect.y && y <= rect.y+rect.h) {
        return 1;
    } else {
        return 0;
    }
}


/*
Modifie la barre de progression en fonction d'un click (ne mets pas à jour, juste modifie les dimension du sub_rect) :
    - Le rect pour la barre (créé et placé en amont)
    - Un pointeur vers le rect pour la progression (créé en amont)
    - Un pointeur pour y enregistré le % de la barre remplie en fontion du clic. Si pas néscessaire de récupérer le % dans le main, mettre NULL
 */
void edit_progress_bar_with_click(SDL_Rect bar, SDL_Rect* sub_bar, float* ratio_bar) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (click_in_rect(bar)) {
        sub_bar->x = bar.x;
        sub_bar->y = bar.y;
        sub_bar->w = x - bar.x;
        sub_bar->h = bar.h;

        if (ratio_bar != NULL) {
            float f_mouse_x = x, f_barre_w = bar.w, f_barre_x = bar.x;
            *ratio_bar = ( (f_mouse_x - f_barre_x) / f_barre_w );
        }
    }
}

/*
Modifie la barre de progression en fonction d'un % souhaité (ne mets pas à jour, juste modifie les dimension du sub_rect) :
    - Le rect pour la barre (créé et placé en amont)
    - Un pointeur vers le rect pour la progression (créé en amont)
    - Le % de la barre qu'on veut remplir
*/
void edit_progress_bar_with_ratio(SDL_Rect bar, SDL_Rect* sub_bar, float ratio) {
    if(ratio>=0 && ratio<=1) {
        sub_bar->x = bar.x;
        sub_bar->y = bar.y;
        sub_bar->w = ratio*bar.w;
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
Va verifier si le clic est dans la box de saisie, et va modifier un pointeur de str avec les entrées du clavier :
    - event qui a été créé dans le main
    - POINTEUR de is_active pour "sauvegarder" dans le main si la box de saisie est cliqué ou pas
    - POINTEUR (définit avec malloc) de la chaine de caractère à modifier (et non pas un tableau de char car ne peut pas etre return)
    - POINtEUR (définit avec malloc) de la "zone" ou sauvegardé une saisie après avoir cliqué ENTER
        - ATTENTION : initialiser le premier caractere des 2 str avec '\0'
    - Le rect dans lequelle doit etre saise le texte (pour détecter si le clic active ou non la box de saisie)
 */


void activate_input_box(SDL_Event event, int* is_active, char* input_text, char* saved_text, SDL_Rect input_box) {
    if(event.type == SDL_TEXTINPUT && *is_active ==1) {
        strcat(input_text, event.text.text);
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN) {
        if(click_in_rect(input_box)) *is_active = 1;
        else *is_active = 0;
    }
    else if(event.type == SDL_KEYDOWN && *is_active == 1) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(input_text)>0) {
            input_text[strlen(input_text) - 1] = '\0';
        }
        else if(event.key.keysym.sym == SDLK_RETURN) {
            strcpy(saved_text, input_text);
            input_text[0] = '\0';
        }
    }
}


/*
Fonction qui met à jour la zone de saisie de texte :
    - Renderer
    - Rect pour la box de saisie (créé et placer en amont)
    - Rect pour la zone de texte (créé avant)        A VOIR POUR LE DEGAGER ET LE CREER DANS LA FONCTION ET PAS DANS LE MAIN
    - Couleur du fond de la box de saisie
    - Police
    - Couleur de police
    - Pointeur pour communiquer le texte saisie (et qui a été saisie) entre la fonction et le main
 */
void maj_input_box(SDL_Renderer* renderer, SDL_Rect box, SDL_Color box_color, char* input_text, TTF_Font* font, SDL_Color font_color) {

    SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
    SDL_RenderFillRect(renderer, &box);

    if(strlen(input_text)>0) {
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, input_text, font_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        SDL_Rect text_rect;
        text_rect.x = box.x+box.w/10;
        text_rect.y = box.y + (box.h - text_surface->h) / 2;
        text_rect.w = text_surface->w;
        text_rect.h = text_surface->h;

        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
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

void create_button(SDL_Renderer* renderer, SDL_Rect button_rect, char text[100], TTF_Font* font, SDL_Color font_color, SDL_Color button_rect_color) {

    SDL_SetRenderDrawColor(renderer, button_rect_color.r, button_rect_color.g, button_rect_color.b, button_rect_color.a);
    SDL_RenderFillRect(renderer, &button_rect);
    SDL_Surface* tmp_surface = TTF_RenderText_Solid(font, text, font_color);
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    SDL_Rect text_rect;
    text_rect.x = button_rect.x + button_rect.w/2 - tmp_surface->w/2;
    text_rect.y = button_rect.y + button_rect.h/2 - tmp_surface->h/2;
    text_rect.w = tmp_surface->w;
    text_rect.h = tmp_surface->h;

    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(tmp_surface);
    SDL_DestroyTexture(text_texture);
}












/*//////////////////////////////////////////////    Partie pour la génération des listes     ////////////////////////////////////////////////////////////////////////////*/


// nb de mots au total : 22 740

// Va chercher le mot à la ligne demander et l'affecte dans mot
void affect_word_from_line(FILE* file, int nb_ligne, char* mot) {
    rewind(file);
    char ligne[50];
    int compteur = 0;
    while(fgets(ligne, 50, file) != NULL && compteur < nb_ligne) {
        compteur++;
        if(compteur == nb_ligne) {
            ligne[strcspn(ligne, "\n")] = '\0';
            strcpy(mot, ligne);
        }
    }
}



// Vérifie si un mot est dans la liste ou pas -> 0 : déja dans la liste     1 : pas là
int check_word_not_used(char** list_to_check, char* word) {
    int not_found = 1, counter = 0;
    while(list_to_check[counter][0] != '\0') {
        if(strcmp(list_to_check[counter], word) == 0) {
            not_found = 0;
        }
        counter++;
    }
    return not_found;
}



// Mélange les liste (conçue pour multi)
void mix_listes(char*** all_player_list, char** common_list, int nb_player, int nb_words) {
    char tmp_list[nb_words][50];
    int index_used[nb_words];
    for(int i=0 ; i<nb_words ; i++) index_used[i] = -1;
    int count = 0;

    // On mélange la liste commune
    for(int i=0 ; i<nb_words ; i++) {
        strcpy(tmp_list[i], common_list[i]);
    }
    while(count<nb_words) {
        int index = rand()%nb_words;
        if(index_used[index] == -1) {
            strcpy(common_list[index], tmp_list[count++]);
            index_used[index] = 0;
        }
    }
    // On mélange les listes joueurs
    for(int i=0 ; i<nb_player ; i++) {
        count = 0;
        for(int j=0 ; j<nb_words ; j++) {
            index_used[j] = -1;
            strcpy(tmp_list[j], all_player_list[i][j]);
        }
        while(count<nb_words) {
            int index = rand()%nb_words;
            if(index_used[index] == -1) {
                strcpy(all_player_list[i][index], tmp_list[count++]);
                index_used[index] = 0;
            }
        }
    }
}




// Fonction qui vérifie si le mot est répété plusieurs fois dans une liste (utilisé pour set_new_player_list pour le mode multi)

int check_word_not_repeat(int nb_words, int nb_player, char all_words[150][50], char* word) {
    int word_valid = 1;
    for(int i=0 ; i<(nb_player+1)*nb_words ; i++) {
        int nb_of_repeat = 0;
        for(int j=i ; j<(nb_player+1)*nb_words ; j++) {
            if(strcmp(word, all_words[j]) == 0) nb_of_repeat++;
        }
        if(nb_of_repeat>1) word_valid = 0;
    }
    return word_valid;
}



// Fonction pour créer la nouvelle liste du joueur gagnant (pour multi) à partir des autres listes et de la nouvelle commune (qui est donc celle du gagnant)

void set_new_player_list(int nb_words, int nb_player, char*** all_player_list, int winning_player) {
    FILE* file = fopen("../mots.txt", "r");

    char tmp_new_list[nb_words][50];

    // Afin d'ajouter un mot "unique", on va mettre tout les mots ensemble, puis on vérifiera si le mot apparait ou non plusieurs fois
    char all_words[nb_player*nb_words+1][50];
    int counter_all_words = 0;

    // On copie les liste des joueur à l'interieur (qui comprendra donc aussi la nouvelle liste commune -> joueur gagnant)
    for(int i=0 ; i<nb_player ; i++) {
        for(int j=0 ; j<nb_words ; j++) {
            strcpy(all_words[counter_all_words++], all_player_list[i][j]);
        }
    }


    // Maintenant on va ajouter un mots de chaque liste (et donc aussi la commune) dans la nouvelle, en vérifiant qu'il est "unique"

    for(int i=0 ; i<nb_player ; i++) {
        int index_in_list = rand()%nb_words;
        while(!check_word_not_repeat(nb_words, nb_player, all_words, all_player_list[i][index_in_list])) {
            index_in_list = rand()%nb_words;
        }
        strcpy(tmp_new_list[i], all_player_list[i][index_in_list]);
    }



    // Et puis on complète le reste de la liste avec des mots qui n'ont pas encore été utilisé
    for(int i=nb_player ; i<nb_words ; i++) {
        char new_word[50];
        int word_valid = 1;
        affect_word_from_line(file, rand()%22741, new_word);
        for(int j=0 ; j<nb_player*nb_words ; j++) {
            if(strcmp(all_words[j], new_word) == 0) word_valid = 0;
        }
        if(word_valid) strcpy(tmp_new_list[i], new_word);
        else i--;
    }

    // Enfin, on copie la tmp_new_list dans la liste du joueur gagnant
    for(int i=0 ; i<nb_words ; i++) {
        strcpy(all_player_list[winning_player][i], tmp_new_list[i]);
    }
}










/* On donne en parametres :
    - Le nb de mots qu'on doit placer
    - La liste du joueur (qui sera directement modifié)
    - La liste commune (idem)
    - Un pointeur qui va être modifé pour donner le mot en commun
    - Un int pour dire si oui (1) ou non (0) il s'agit de la 1ère initialisation des listes
 */
void generate_list_solo(int nb_words, char** player_list, char** common_list, char* common_word, int init_list) {

    FILE* file = fopen("../mots.txt", "r");
    srand(time(NULL) ^ getpid());   // Initialisatin du générateur de nb aléatoire (pour qu'ils soient un peu + aléatoire)

    // Création d'un tableau qui va comporter tout les mots déja utilisé (et son counter)
    char* used_words[2*nb_words];
    int counter_used_words = 0;

    // Allocation de la place que néscessite le tab de STR used_words
    for(int i=0 ; i<(2*nb_words) ; i++) {
        used_words[i] = malloc(sizeof(char)*50);
        used_words[i][0] = '\0';
    }

    // Si c'est la première initialisation
    if(init_list) {
        // Génération des 2 listes (+ enregistrement à chaque fois dans used_words) sans aucune redondance de mots
        for(int i=0 ; i<nb_words ; i++) {
            char player_list_word[50] = "";
            char common_list_word[50] = "";
            affect_word_from_line(file, rand()%22741, player_list_word);
            affect_word_from_line(file, rand()%22741, common_list_word);
            // Si les deux mots ne sont pas encore utilisé ET qu'ils sont différent l'un par rapport à l'autre --> ajouter dans les liste + save in used_words
            if(check_word_not_used(used_words, player_list_word) && check_word_not_used(used_words, common_list_word) && strcmp(player_list_word, common_list_word) != 0) {
                strcpy(used_words[counter_used_words++], player_list_word);
                strcpy(used_words[counter_used_words++], common_list_word);
                strcpy(player_list[i], player_list_word);
                strcpy(common_list[i], common_list_word);
            }
            // Si on a pas pu affecter un nouveau mot, on "recommence"
            else i--;
        }

        // Génération du mot en commun, puis on remplace un des mots généré avant (indice aléatoire) par ce mot commun
        affect_word_from_line(file, rand()%22741, common_word);
        strcpy(player_list[rand()%(nb_words)], common_word);
        strcpy(common_list[rand()%(nb_words)], common_word);
    }

    // Sinon on met à jour par rapport à la liste joueur
    else {
        // Remplacement de la liste commune par l'ancienne joueur (et on mets les mots dans used_words
        for(int i=0 ; i<nb_words ; i++) {
            strcpy(common_list[i], player_list[i]);
            strcpy(used_words[counter_used_words++], player_list[i]);
        }
        // On génère une nouvelle liste joueur en vérifiant que les mots n'ont pas déja été utilisé
        for(int i=0 ; i<nb_words ; i++) {
            char player_list_word[50] = "";
            affect_word_from_line(file, rand()%22741, player_list_word);
            if(check_word_not_used(used_words, player_list_word)) {
                strcpy(used_words[counter_used_words++], player_list_word);
                strcpy(player_list[i], player_list_word);
            }
            // Si on a pas pu affecter un nouveau mot, on "recommence"
            else i--;
        }
        // Génération du mot en commun, puis on remplace un des mots généré avant (indice aléatoire) par ce mot commun
        affect_word_from_line(file, rand()%22741, common_word);
        strcpy(player_list[rand()%(nb_words)], common_word);
        strcpy(common_list[rand()%(nb_words)], common_word);
    }

    // Libération de la mémoire
    for(int i=0 ; i<(2*nb_words) ; i++) free(used_words[i]);
}





/*
On donne en parametre :
    - le nb de mots
    - le nb de joueur
    - la liste comprtant toute les listes des joueurs
    - la liste commune
    - si oui (1) ou non (0) il s'agit de la première initilaistion des listes
    - Si on met juste à jour, indiquer le numéro du joueur gagnant
 */


void generate_list_multi(int nb_words, int nb_player, char*** all_player_list, char** common_list, int init, int winning_player) {
    FILE* file = fopen("../mots.txt", "r");
    srand(time(NULL) ^ getpid());   // Initialisatin du générateur de nb aléatoire (pour qu'ils soient un peu + aléatoire)

    if(init) {
        // Création d'un tab contenant tout les mots utilisés
        char* used_words[nb_words*(nb_player+1)];
        for(int i=0 ; i<nb_words*(nb_player+1) ; i++) {
            used_words[i] = malloc(sizeof(char)*50);
            used_words[i][0] = '\0';
        }
        int counter_used_words = 0;


        // Génération de toute les listes, d'abord sans aucun mots en commun

        // Génération de la liste commune
        for(int i=0 ; i<nb_words ; i++) {
            char common_list_word[50] = "";
            affect_word_from_line(file, rand()%22741, common_list_word);
            if(check_word_not_used(used_words, common_list_word)) {
                strcpy(used_words[counter_used_words++], common_list_word);
                strcpy(common_list[i], common_list_word);
            }
            // Si on a pas pu affecter un nouveau mot, on "recommence"
            else i--;
        }

        // Génération des listes des joueurs
        for(int i=0 ; i<nb_player ; i++) {
            for(int j=0 ; j<nb_words ; j++) {
                char player_word[50] = "";
                affect_word_from_line(file, rand()%22741, player_word);
                if(check_word_not_used(used_words, player_word)) {
                    strcpy(all_player_list[i][j], player_word);
                    strcpy(used_words[counter_used_words++], player_word);
                }
                else j--;
            }
        }


        // On va placer les mots en commun

        // On commence avec la liste commune
        for(int i=0 ; i<nb_player ; i++) {
            strcpy(all_player_list[i][0], common_list[i]);
        }

        // On continue avec toute les listes
        int index = 1;
        for(int i=0 ; i<nb_player ; i++) {
            for(int j=i+1 ; j<nb_player ; j++) {
                strcpy(all_player_list[j][index], all_player_list[i][index++]);
            }
            index=1;
        }

        // On mélange les listes
        mix_listes(all_player_list, common_list, nb_player, nb_words);
    }

    // S'il s'agit juste d'une mise à jour suite à une victoire
    else {

        // On copie la liste du joueur gagnant dans la liste commune
        for(int i=0 ; i<nb_words ; i++) {
            strcpy(common_list[i], all_player_list[winning_player][i]);
        }

        // Pour la nouvelle liste, on sélectionne 1 mot de chaque liste (dont donc la nouvelle commune) qui n'est dans aucune autre liste, qui est "unique"
        set_new_player_list(nb_words, nb_player, all_player_list, winning_player);
        }
    }
