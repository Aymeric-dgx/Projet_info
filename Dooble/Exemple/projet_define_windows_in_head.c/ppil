#include <stdio.h>
#include <stdlib.h>

// Si test est un pointeur vers un structure : test->a = 1    <==>    (*test).a = 1

// Si je declare une strucuture comme ceci : 
/*
typedef struct tmp {    //tmp est le nom de la strucutre
    int a;
    int b;
} second_name;        // second_name est le nom a utiliser si on veut créer la structure sans utiliser struct avant
*/ 
// Ainsi : struct tmp Structure1    <==>    second_name tmp Structure1


// Difference entre allouer de la mémoire dynamiquement et statiquement
// Dynamiquement : Les données peuvent exister au-delà de la portée de la fonction qui les a créées. Elles ne sont libérées que lorsqu'on appele free.
// Statiquement : Les données sont détruites à la fin de la fonction qui les a créées.
// Vérifier car pas encore testé/manipulé

// On va créer un void tableau de pointeurs qui vont pointer vers chaque strucutre différente
// Si on avait simplement fait un tableau de strcuture, on aurait pu avoir des problemes car chaque strucutre prendrait + ou - d escpace --> il faut faire au cas par cas
// Ainsi en faisant un tableau de pointeurs de strucutre, comme chaque pointeurs à une taille relativement fixe, dans le "gros" save on aura pas de pb de mémoire
// Et de plus, cela nous permettra d'allouer au cas par cas la mémoire nescessaire, comme ici par exemple :
// struct Test* tmp = malloc(sizeof(struct Test));

// NB : malloc retourne un pointeur qui pointe vers un début de bloc mémoire (de la taille qu'on aura définit) et qui peut etre considéré comme un tableau (tant qu'on ne change pas l'adresse qu'il pointe) 
// Ainsi si on créer un tableau de pointeur avec malloc, il faut mettre (void**) malloc... afin de préciser que malloc n'est pas un pointeur mais un pointeur de pointeur

// Explication + exemple Copilot à la question : 
/*
ok donc on fait void** array (void**) malloc(...) parce que comme ça on dit que malloc est un pointeur de pointeur et donc si je fais array[2] l'ordinateur 
comprendra qu'on pointe vers un pointeur ?
*/

/* 
Oui, vous avez bien compris le principe général. Utiliser void** array = (void**) malloc(...) permet de dire que array est un pointeur vers un pointeur de type void. 
Cela signifie que array peut être utilisé comme un tableau de pointeurs de type void*.

Voici une explication détaillée :

Déclaration :

void** array;
array est un pointeur vers un pointeur de type void.
Allocation de mémoire :

array = (void**) malloc(sizeof(void*) * 2);
malloc(sizeof(void*) * 2) alloue de la mémoire pour un tableau de 2 pointeurs de type void*.
(void**) convertit le résultat de malloc (qui est de type void*) en void**.
Utilisation :

Vous pouvez maintenant utiliser array comme un tableau de pointeurs de type void*.
Par exemple, array[2] accède au troisième élément du tableau de pointeurs.
*/

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
