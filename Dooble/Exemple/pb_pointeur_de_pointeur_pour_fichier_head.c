#include <stdio.h>
#include <stdlib.h>


// Difference entre allouer de la mémoire dynamiquement et statiquement
// Dynamiquement : Les données peuvent exister au-delà de la portée de la fonction qui les a créées. Elles ne sont libérées que lorsqu'on appele free.
// Statiquement : Les données sont détruites à la fin de la fonction qui les a créées.



struct Test {
    int a;
    int b;
};

struct Test2 {
    int a;
    int b;
};


// Retourne un tabeau de pointeur
void** test() {
    // On définit des pointeurs de structure et non pas des strucutures, car sinon ces structure seront détruite à la fin de la fonction
    // Là où avec des pointeurs on pourra (si on les renvoie) réutiliser ces dernierse afin de "retrouver" les strucutres créées/modifiées dans cette fonction
    struct Test* tmp = malloc(sizeof(struct Test));     // Bien alloué dynamiquement la mémoire et pas seulement faire tmp[10]
    struct Test2* tmp2 = malloc(sizeof(struct Test2));  // Car si on fait comme ça, la variable sera supprimé à la fin de la fonction ?
    // Retourne des poiteurs vers la mémoire contenant les structures
    tmp->a = 1;     // Initialise le membre 'a' de struct Test à 1
    tmp->b = 2;     // Initialise le membre 'b' de struct Test à 2
    tmp2->a = 3;    // Initialise le membre 'a' de struct Test2 à 3
    tmp2->b = 4;    // Initialise le membre 'b' de struct Test2 à 4
    // On utilise ->a au lieu de .a car ce sont des pointeurs vers des structures et non pas directement des structures

    void** save = malloc(20 * sizeof(void*)); //  Alloue de la mémoire pour un tableau de 20 pointeurs (de type void*, car permet ainsi de stocker n'importe quel type de pointeur)
    // Stocke les adresses des structures tmp et tmp2 dans le tableau save
    save[0] = tmp;
    save[1] = tmp2;

    return save;    // Retourne le tableau de pointeurs
}

int main() {
    void** new_save = test();   // Appelle la fonction test et stocke le tableau de pointeurs retourné dans new_save
    // On convertit le pointeur de type void* en pointeur de type struct Test* pour pouvoir accéder aux membres de la structure
    // Nb : vu que first va etre un POINTEUR de strucutrure (et non pas directement une structure), on doit utiliser ->a et non pas .a 
    struct Test* first = (struct Test*)new_save[0]; // On cast le pointeur de type void* en pointeur de type struct Test*
    int a = first->a;

    printf("test %d\n", a);

    // On libère TOUTE les mémoire allouée dynamiquement
    free(new_save[0]);  // Libère la mémoire allouée pour la structure tmp (dans test)
    free(new_save[1]);  // Libère la mémoire allouée pour la structure tmp2 (dans test)
    free(new_save);    // Libère la mémoire allouée pour le tableau de pointeurs save (dans test)

    return 0;
}
