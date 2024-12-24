#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

// nb de mots au total : 22 740

// Va cherhcer le mot à la ligne demander et l'affecte dans mot
void affect_word_from_line(FILE* file, int nb_ligne, char* mot) {
    rewind(file);
    int compteur = 0;
    while(fgets(mot, 50, file) != NULL && compteur < nb_ligne) {
        compteur++;
        if(compteur == nb_ligne) return;
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

void generate_list_solo(FILE* file, int nb_words) {

    srand(time(NULL) ^ getpid());   // Initialisatin du générateur de nb aléatoire (pour qu'ils soient un peu + aléatoire)

    // Création des poiteurs "intereurs" de la fonction (à la fin on affectera certaines de leurs adresses aux pointeurs d'entrée de la fonctions)
    char** player_list = malloc(sizeof(char*)*nb_words);
    char** common_list = malloc(sizeof(char*)*nb_words);
    char** used_words = malloc(sizeof(char*)*(2*nb_words));
    char* common_word = malloc(sizeof(char)*50);

    int counter_used_words = 0;

    // Initialisation des pointeurs de pointeurs de char (équivalent à des tabs de str)
    for(int i=0 ; i<nb_words ; i++) {
        player_list[i] = malloc(sizeof(char)*50);
        common_list[i] = malloc(sizeof(char)*50);
        player_list[i][0] = '\0';
        common_list[i][0] = '\0';
    }
    for(int i=0 ; i<(2*nb_words) ; i++) {
        used_words[i] = malloc(sizeof(char)*50);
        used_words[i][0] = '\0';
    }

    char* player_list_word = malloc(sizeof(char)*50);
    char* common_list_word = malloc(sizeof(char)*50);
    // Génération des 2 listes (+ enregistrement à chaque fois dans used_words) avec aucune redondance de mots
    for(int i=0 ; i<nb_words ; i++) {
        affect_word_from_line(file, rand()%22741, player_list_word);
        affect_word_from_line(file, rand()%22741, common_list_word);
        // Si les deux mots ne sont pas encore utilisé ET qu'ils sont différent l'un par rapport à l'autre --> ajouter dans les liste + save in used_words
        if(check_word_not_used(used_words, player_list_word) && check_word_not_used(used_words, common_list_word)) {
            used_words[counter_used_words++] = player_list_word;
            used_words[counter_used_words++] = common_list_word;
            player_list[i] = player_list_word;
            common_list[i] = common_list_word;
            printf("hello %d\n", i);
        }
        printf("HEY %d\n", i);
    }

    /*// Génération du mot en commun (on "force" pour le placer afin que il y ai UN seul mot en commun)
    affect_word_from_line(file, rand()%22741, common_word);
    strcpy(used_words[counter_used_words++], common_word);
    strcpy(player_list[rand()%(nb_words+1)], common_word);
    strcpy(common_list[rand()%(nb_words+1)], common_word);
*/

    for (int i=0 ; i<nb_words ; i++) {
        free(player_list[i]);
        free(common_list[i]);
    }
    for(int i=0 ; i<nb_words ; i++) {
        free(used_words[i]);
    }

    free(player_list);
    free(common_list);
    free(used_words);
    free(common_word);
}


int main() {
    srand(time(NULL) ^ getpid());   // Initialisatin du générateur de nb aléatoire (pour qu'ils soient un peu + aléatoire)
    FILE* file = fopen("../mots.txt", "r");


    generate_list_solo(file, 7);
}
