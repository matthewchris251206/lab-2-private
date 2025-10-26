#include "print.h"
#include "memory.h"

int main(void) {
    my_alloc_init();
    int *a = my_malloc(sizeof(int));
    *a     = 5;
    my_printf("a: %d\n", *a);
    *a = 10;
    my_printf("a: %d\n", *a);
    int *b = my_malloc(sizeof(int));
    *b     = 15;
    my_printf("b: %d\n", *b);
    *b = 20;
    my_printf("b: %d\n", *b);
    my_free(a);
    my_free(b);
}
