#include <stdio.h>
#include <stdlib.h>

void interchange(int *list, size_t size) {
    if (list == NULL || size < 2) {
        return;
    }
    int temp = list[0];
    list[0] = list[size - 1];
    list[size - 1] = temp;
}

int main(void) {
    size_t size = 5;
    int *list = malloc(size * sizeof(int));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    list[0] = 1;
    list[1] = 2;
    list[2] = 3;
    list[3] = 4;
    list[4] = 5;

    for (size_t i = 0; i < size; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");

    interchange(list, size);

    for (size_t i = 0; i < size; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");

    free(list);
    list = NULL;

    return EXIT_SUCCESS;
}