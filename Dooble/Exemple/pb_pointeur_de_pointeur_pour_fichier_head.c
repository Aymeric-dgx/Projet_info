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

void** test() {
    struct Test* tmp = malloc(sizeof(struct Test));
    struct Test2* tmp2 = malloc(sizeof(struct Test2));
    tmp->a = 1;
    tmp->b = 2;
    tmp2->a = 3;
    tmp2->b = 4;

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
