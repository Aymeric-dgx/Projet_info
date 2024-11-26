#include <stdio.h>
#include <stdlib.h>

typedef struct tmp {
    int a;
    int b;
} tmp;

void** test() {
    void** save = malloc(sizeof(void*) * 2);    // Tableau de pointeurs
    struct tmp* test1 = malloc(sizeof(struct tmp)); // Pointeur de structure
    struct tmp* test2 = malloc(sizeof(struct tmp)); // Pointeur de structure

    test1->a = 1;   // equivalent à faire (*test1).a = 1;
    test1->b = 2;
    test2->a = 3;
    test2->b = 4;

    save[0] = test1;
    save[1] = test2;

    return save;
}

int main() {
    void** new_save = test();

    tmp* t1 = new_save[0];
    /* On doit faire ça car new_save[0] est un void*, et on doit donc préciser que c'est un tmp* (càd un pointeur de structure tmp)
     Ici donc on créer un pointeur de structure tmp qui a pour valeur (= adresse) new_save[0] (qui est un void*) */

    ((tmp*)new_save[1])->a = 5; // On peut aussi faire ça, mais c'est moins lisible : on cast (=convertir) new_save[1] (qui est un void*) en tmp* (pointeur de structure tmp) et on accède à l'attribut a

    printf("%d %d %d %d\n", t1->a, t1->b, ((tmp*)new_save[1])->a, ((tmp*)new_save[1])->b);


    // Libérer la mémoire prise par save, test1 et test2
    free(new_save[0]);
    free(new_save[1]);
    free(new_save);

}
