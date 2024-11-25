#include <stdio.h>
#include <stdlib.h>

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
    // On définit des poitneurs de structure et non pas des strucutures, car sinon ces structure seront détruite à la fin de la fonction
    // Là où avec des pointeurs je pourrai (si je les renvoie) réutiliser ses dernierse afin de "retrouver" les strucutres créés dans cette fonction
    struct Test* tmp = malloc(sizeof(struct Test));     // Bien alloué dynamiquement la mémoire et pas seulement faire tmp[10]
    struct Test2* tmp2 = malloc(sizeof(struct Test2));  // Car si on fait comme ça, la variable sera supprimé à la fin de la fonction ?
    // Retourne des poiteurs vers la mémoire contenant les structures
    tmp->a = 1;     // Initialise le membre 'a' de struct Test à 1
    tmp->b = 2;     // Initialise le membre 'b' de struct Test à 2
    tmp2->a = 3;    // Initialise le membre 'a' de struct Test2 à 3
    tmp2->b = 4;    // Initialise le membre 'b' de struct Test2 à 4
    // On utilise ->a au lieu de .a car ce sont des pointeurs vers des structures et non pas directement des structures

    void** save = malloc(20 * sizeof(void*));
    save[0] = tmp;
    save[1] = tmp2;

    return save;
}

int main() {
    void** new_save = test();
    struct Test* first = (struct Test*)new_save[0];
    int a = first->a;

    printf("test %d\n", a);

    free(new_save[0]);
    free(new_save[1]);
    free(new_save);

    return 0;
}
