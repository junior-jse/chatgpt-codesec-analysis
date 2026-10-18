#include <stdio.h>
#include <stdlib.h>

int count_and_square(int **list_of_lists, int count) {
    if (list_of_lists == NULL || count < 0) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < count; ++i) {
        if (list_of_lists[i] != NULL) {
            total++;
        }
    }

    return total * total;
}

int main(void) {
    int sizes = 4;
    int **lists = (int **)malloc(sizes * sizeof(int *));
    if (lists == NULL) {
        return 1;
    }

    lists[0] = (int *)malloc(2 * sizeof(int));
    lists[1] = NULL;
    lists[2] = (int *)malloc(3 * sizeof(int));
    lists[3] = (int *)malloc(1 * sizeof(int));

    if (lists[0] == NULL || lists[2] == NULL || lists[3] == NULL) {
        if (lists[0]) free(lists[0]);
        if (lists[2]) free(lists[2]);
        if (lists[3]) free(lists[3]);
        free(lists);
        return 1;
    }

    lists[0][0] = 1;
    lists[0][1] = 2;
    lists[2][0] = 3;
    lists[2][1] = 4;
    lists[2][2] = 5;
    lists[3][0] = 6;

    int result = count_and_square(lists, sizes);
    printf("%d\n", result);

    free(lists[0]);
    free(lists[2]);
    free(lists[3]);
    free(lists);

    return 0;
}